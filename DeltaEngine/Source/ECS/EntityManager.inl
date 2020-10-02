namespace DeltaEngine::ECS_Internal
{
template <typename... Ts>
struct TypeList
{};

template <typename T, typename Ret, typename... Args>
TypeList<Args...> args( Ret( T:: * )( Args... ) const );

template <typename... Args, typename Func>
void chunk_iterate( DataChunk *chunk, Func &&func )
{
  auto tuple = std::make_tuple( get_chunk_array<Args>( chunk )... );

#ifndef NDEBUG
  ( assert( std::get<decltype( get_chunk_array<Args>( chunk ) )>( tuple ).chunk_owner() == chunk ), ... );
#endif

  for ( int i = static_cast<int>( chunk->header.index ) - 1; i >= 0; i-- )
    func( std::get<decltype( get_chunk_array<Args>( chunk ) )>( tuple )[i]... );
}

template <typename... Args, typename Func>
void unpack_chunk( TypeList<Args...>, DataChunk *chunk, Func &&func )
{
  chunk_iterate<Args...>( chunk, func );
}

template <typename... Args>
Query &unpack_query_with( TypeList<Args...>, Query &query )
{
  return query.with<Args...>();
}

template <typename T>
inline auto get_chunk_array( DataChunk *chunk )
{
  using T_Base = std::remove_const_t<std::remove_reference_t<T>>;

  if constexpr ( std::is_same_v<T_Base, EntityID> )
  {
    EntityID *ptr = reinterpret_cast<EntityID *>( chunk );
    return ArrayView<EntityID>( ptr, chunk );
  }
  else
  {
    constexpr MetaHash hash = Metatype::build_hash<T_Base>();

    for ( auto comp : chunk->header.owner->components->metatypes )
      if ( comp.type->hash == hash )
      {
        T_Base *ptr = reinterpret_cast<T_Base *>( reinterpret_cast<byte *>( chunk ) + comp.offset );
        return ArrayView<T_Base>( ptr, chunk );
      }

    return ArrayView<T_Base>();
  }
}
} // namespace DeltaEngine::ECS_Internal

namespace DeltaEngine
{

  //******************************************************************************
  // EntityManager Public Methods
  //******************************************************************************

EntityManager::EntityManager()
{
  Archetype *empty_arch = new Archetype();

  empty_arch->full_chunks = 0;
  empty_arch->components = build_component_list( nullptr, 0 );
  empty_arch->component_hash = 0;
  empty_arch->owner = this;

  archetypes.push_back( empty_arch );
  archetypes_signature.push_back( 0 );

  create_chunk( empty_arch );
}

EntityManager::~EntityManager()
{
  for ( Archetype *arch : archetypes )
  {
    for ( DataChunk *chunk : arch->chunks )
      delete chunk;
    delete arch;
  }
}

template <typename... C>
EntityID EntityManager::create_entity()
{
  Archetype *arch { nullptr };

  if constexpr ( sizeof...( C ) != 0 )
  {
    static const Metatype *types[] = { Metatype::get_metatype<C>()... };
    constexpr size_t count = ( sizeof( types ) / sizeof( *types ) );

    arch = find_or_create_archetype( types, count );
  }
  else
    arch = get_empty_archetype();

  EntityID id = allocate_entity();
  set_entity_archetype( id, arch );

  return id;
}

void EntityManager::destroy_entity( EntityID id )
{
  assert( is_entity_valid( id ) );
  erase_entity_chunk( entities[id.index].chunk, entities[id.index].chunk_index );
  deallocate_entity( id );
}

template <typename C>
bool EntityManager::has_component( EntityID id )
{
  Entity &ref = entities[id.index];
  auto c_array = get_chunk_array<C>( ref.chunk );
  return c_array.owner != nullptr;
}

template <typename C>
C &EntityManager::get_component( EntityID id )
{
  Entity &ref = entities[id.index];

  auto c_array = ECS_Internal::get_chunk_array<C>( ref.chunk );
  assert( c_array.chunk_owner() != nullptr );
  return c_array[ref.chunk_index];
}

template <typename C>
void EntityManager::add_component( EntityID id, C &comp )
{
  add_component<C>( id );

  if ( !Metatype::get_metatype<C>()->is_empty() )
    get_component<C>( id ) = comp;
}

template <typename C>
void EntityManager::add_component( EntityID id )
{
  const Metatype *temp_meta_array[32];
  const Metatype *type = Metatype::get_metatype<C>();

  Archetype *arch = entities[id.index].chunk->header.owner;
  ComponentList *list = arch->components;

  bool found { false };
  size_t count = list->metatypes.size();

  for ( size_t i = 0; i < count; i++ )
  {
    temp_meta_array[i] = list->metatypes[i].type;

    if ( temp_meta_array[i] == type )
      found = true;
  }

  if ( !found )
  {
    temp_meta_array[count] = type;
    arch = find_or_create_archetype( temp_meta_array, ++count );
    set_entity_archetype( id, arch );
  }
}

template <typename C>
void EntityManager::remove_component( EntityID id )
{
  const Metatype *temp_meta_array[32] { nullptr };
  const Metatype *type = Metatype::get_metatype<C>();

  Archetype *arch = entities[id.index].chunk->header->owner;
  ComponentList *list = old_arch->components;

  bool found { false };
  size_t count = list->metatypes.size();

  for ( size_t i = 0; i < count; i++ )
  {
    temp_meta_array[i] = list->metatypes[i].type;

    if ( temp_meta_array[i] == type )
    {
      found = true;
      temp_meta_array[i] = list->metatypes[count - 1].type;
    }
  }

  if ( found )
  {
    arch = find_or_create_archetype( *this, temp_meta_array, --count );
    set_entity_archetype( arch, id );
  }
}

template <typename Func>
void EntityManager::for_each( Func &&func )
{
  using params = decltype( ECS_Internal::args( &Func::operator() ) );

  Query query;
  ECS_Internal::unpack_query_with( params {}, query ).build();

  for_each<Func>( query, std::move( func ) );
}

template <typename Func>
void EntityManager::for_each( Query &query, Func &&func )
{
  using params = decltype( ECS_Internal::args( &Func::operator() ) );

  archetype_iterate( query, [&]( Archetype *arch )
  {
    for ( auto chunk : arch->chunks )
      ECS_Internal::unpack_chunk( params {}, chunk, func );
  } );
}

Archetype *EntityManager::get_empty_archetype()
{
  return archetypes[0];
}

//******************************************************************************
// EntityManager Private Methods
//******************************************************************************

bool EntityManager::is_entity_valid( EntityID id )
{
  if ( entities.size() > id.index )
    if ( entities[id.index].generation == id.generation )
      return true;
  return false;
}

EntityID EntityManager::allocate_entity()
{
  EntityID id;
  if ( entities_dead == 0 )
  {
    size_t index = entities.size();

    Entity new_entity;
    entities.push_back( new_entity );

    id.index = index;
  }
  else
  {
    size_t index = entities_deleted.back();
    entities_deleted.pop_back();
    id.generation = entities[index].generation;
    id.index = index;
    entities_dead--;
  }

  entities_live++;
  return id;
}

void EntityManager::deallocate_entity( EntityID id )
{
  entities_deleted.push_back( id.index );
  entities[id.index].generation++;
  entities[id.index].chunk = nullptr;
  entities[id.index].chunk_index = 0;

  entities_live--;
  entities_dead++;
}

size_t EntityManager::insert_entity_chunk( DataChunk *chunk, EntityID id, bool initialize )
{
  size_t index { 0 };

  ComponentList *list = chunk->header.owner->components;

  if ( chunk->header.index < list->capacity )
  {
    index = chunk->header.index++;

    if ( initialize )
    {
      for ( auto &ref : list->metatypes )
      {
        const Metatype *type = ref.type;

        if ( !type->is_empty() )
        {
          void *ptr = static_cast<void *>( reinterpret_cast<byte *>( chunk ) + ref.offset + ( type->size * index ) );
          type->constructor( ptr );
        }
      }
    }

    EntityID *id_ptr = reinterpret_cast<EntityID *>( chunk );
    id_ptr[index] = id;

    if ( chunk->header.index == list->capacity )
      set_chunk_full( chunk );
  }

  return index;
}

EntityID EntityManager::erase_entity_chunk( DataChunk *chunk, size_t index )
{
  assert( chunk->header.index > index );

  ComponentList *list = chunk->header.owner->components;

  bool full = chunk->header.index == list->capacity;
  bool pop = chunk->header.index > 1 && index != ( chunk->header.index - 1 );

  size_t pop_index = chunk->header.index - 1;

  chunk->header.index--;

  for ( auto &ref : list->metatypes )
  {
    const auto [type, offset] = ref;

    if ( !type->is_empty() )
    {
      void *ptr = static_cast<void *>( reinterpret_cast<byte *>( chunk ) + offset + ( type->size * index ) );
      type->destructor( ptr );

      if ( pop )
      {
        void *pop_ptr = static_cast<void *>( reinterpret_cast<byte *>( chunk ) + offset + ( type->size * pop_index ) );
        std::memcpy( ptr, pop_ptr, type->size );
      }
    }
  }

  EntityID *id_ptr = reinterpret_cast<EntityID *>( chunk );
  id_ptr[index] = EntityID {};

  if ( chunk->header.index == 0 )
    delete_chunk( chunk );
  else if ( full )
    set_chunk_partial( chunk );

  if ( pop )
  {
    entities[id_ptr[pop_index].index].chunk_index = index;
    id_ptr[index] = id_ptr[pop_index];
    return id_ptr[index];
  }

  return EntityID {};
}

void EntityManager::set_entity_archetype( EntityID id, Archetype *arch )
{
  if ( !entities[id.index].chunk )
  {
    DataChunk *chunk = find_free_chunk( arch );

    size_t index = insert_entity_chunk( chunk, id );
    entities[id.index].chunk = chunk;
    entities[id.index].chunk_index = index;
  }
  else
    move_entity_to_archetype( id, arch, false );
}

void EntityManager::move_entity_to_archetype( EntityID id, Archetype *arch, bool initialize )
{
  DataChunk *current_chunk = entities[id.index].chunk;
  DataChunk *target_chunk = find_free_chunk( arch );

  size_t current_index = entities[id.index].chunk_index;
  size_t target_index = insert_entity_chunk( target_chunk, id, initialize );

  ComponentList *current_list = current_chunk->header.owner->components;
  ComponentList *target_list = target_chunk->header.owner->components;

  for ( size_t i = 0; i < current_list->metatypes.size(); i++ )
  {
    auto [current_type, current_offset] = current_list->metatypes[i];
    if ( !current_type->is_empty() )
      for ( size_t j = 0; j < target_list->metatypes.size(); j++ )
      {
        auto [target_type, target_offset] = target_list->metatypes[j];

        if ( current_type == target_type )
        {
          void *current = static_cast<void *>(
            reinterpret_cast<byte *>( current_chunk ) +
            current_offset +
            ( current_type->size * current_index ) );

          void *target = static_cast<void *>(
            reinterpret_cast<byte *>( target_chunk ) +
            target_offset +
            ( target_type->size * target_index ) );

          std::memcpy( target, current, current_type->size );
        }
      }
  }

  erase_entity_chunk( current_chunk, current_index );

  entities[id.index].chunk = target_chunk;
  entities[id.index].chunk_index = target_index;
}

Archetype *EntityManager::find_or_create_archetype( const Metatype **types, size_t count )
{
  assert( count < 32 );

  const size_t signature = Metatype::build_signature( types, count );

  for ( size_t i = 0; i < archetypes_signature.size(); i++ )
    if ( archetypes_signature[i] == signature )
    {
      ComponentList *list = archetypes[i]->components;
      if ( list->metatypes.size() == count )
      {
        for ( size_t j = 0; j < count; j++ )
          if ( list->metatypes[j].type != types[j] )
            break;

        return archetypes[i];
      }
    }

  // else

  Archetype *new_arch = new Archetype();
  new_arch->components = build_component_list( types, count );
  new_arch->component_hash = signature;
  new_arch->owner = this;

  archetypes.push_back( new_arch );
  archetypes_signature.push_back( signature );

  create_chunk( new_arch );
  return new_arch;
}

template <typename Func>
void EntityManager::archetype_iterate( const Query &query, Func &&func )
{
  for ( size_t i = 0; i < archetypes_signature.size(); i++ )
  {
    if ( archetypes_signature[i] & query.required_matcher_hash )
    {
      ComponentList *list = archetypes[i]->components;
      if ( archetypes_signature[i] & query.excluded_matcher_hash )
      {
        bool skip { false };
        [&]()
        {
          for ( auto excluded : query.excluded )
            for ( auto types : list->metatypes )
              if ( types.type->hash == excluded )
              {
                skip = true;
                return;
              }
        }( );
        if ( skip )
          continue;
      }
      size_t match = std::count_if( query.required.begin(), query.required.end(), [&]( MetaHash hash )
      {
        for ( auto types : list->metatypes )
          if ( hash == types.type->hash )
            return true;
        return false;
      } );

      if ( match == query.required.size() )
        func( archetypes[i] );
    }
  }
}

ComponentList *EntityManager::build_component_list( const Metatype **types, size_t count )
{
  ComponentList *list = new ComponentList();

  size_t size = sizeof( EntityID );
  for ( size_t i = 0; i < count; i++ )
    size += types[i]->size;

  size_t storage_space = sizeof( DataChunk::data );

  size_t slots = ( storage_space / size ) - 1;

  size_t offset = sizeof( DataChunkHeader );
  offset += sizeof( EntityID ) * slots;

  for ( size_t i = 0; i < count; i++ )
  {
    const Metatype *type = types[i];
    if ( type->align != 0 )
    {
      size_t r = offset % type->align;
      size_t type_offset = type->align - r;
      offset += type_offset;
    }

    list->metatypes.push_back( { type, offset } );

    if ( type->align != 0 )
      offset += type->size * slots;
  }

  assert( offset <= MEMORY_BLOCK_16K );

  list->capacity = slots;

  return list;
}

void EntityManager::reorder_chunk( Archetype *arch )
{
  size_t size = arch->components->capacity;
  std::partition( arch->chunks.begin(), arch->chunks.end(), [size]( DataChunk *chnk )
  {
    return ( chnk->header.index == size );
  } );
}

void EntityManager::set_chunk_full( DataChunk *chunk )
{
  Archetype *arch = chunk->header.owner;
  arch->full_chunks++;

  reorder_chunk( arch );
}

void EntityManager::set_chunk_partial( DataChunk *chunk )
{
  Archetype *arch = chunk->header.owner;
  arch->full_chunks--;

  reorder_chunk( arch );
}

DataChunk *EntityManager::find_free_chunk( Archetype *arch )
{
  DataChunk *chunk { nullptr };
  if ( arch->chunks.size() == 0 )
    chunk = create_chunk( arch );
  else
  {
    chunk = arch->chunks[arch->chunks.size() - 1];
    if ( chunk->header.index == arch->components->capacity )
      chunk = create_chunk( arch );
  }
  return chunk;
}

DataChunk *EntityManager::create_chunk( Archetype *arch )
{
  DataChunk *chunk = new DataChunk();

  chunk->header.owner = arch;
  arch->chunks.push_back( chunk );
  return chunk;
}

void EntityManager::delete_chunk( DataChunk *chunk )
{
  Archetype *owner = chunk->header.owner;
  DataChunk *backChunk = owner->chunks.back();

  if ( backChunk != chunk )
    for ( int i = 0; i < owner->chunks.size(); i++ )
      if ( owner->chunks[i] == chunk )
        owner->chunks[i] = backChunk;

  owner->chunks.pop_back();
  delete chunk;
}

} // namespace DeltaEngine