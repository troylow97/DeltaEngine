#include "Core/Debugging/Assert.h"

namespace DeltaEngine::ECS_Internal
{
template <typename... Ts>
struct TypeList
{};

template <typename T, typename Ret, typename... Args>
TypeList<Args...> args( Ret( T:: * )( Args... ) const );

template <typename T>
auto get_chunk_array( DataChunk *chunk )
{
  using T_Base = std::remove_const_t<std::remove_reference_t<T>>;

  if constexpr ( std::is_same_v<T_Base, EntityID> )
  {
    EntityID *ptr = reinterpret_cast<EntityID *>( chunk );
    return ArrayView<EntityID>( ptr, chunk );
  }
  else
  {
    constexpr MetaHash hash = Metatype::BuildHash<T_Base>();

    for ( auto comp : chunk->header.owner->components->metatypes )
      if ( comp.type->hash == hash )
      {
        T_Base *ptr = reinterpret_cast<T_Base *>( reinterpret_cast<byte *>( chunk ) + comp.offset );
        return ArrayView<T_Base>( ptr, chunk );
      }
    return ArrayView<T_Base>();
  }
}

template <typename... Args, typename Func>
void chunk_iterate( DataChunk *chunk, Func &&func )
{
  auto tuple = std::make_tuple( get_chunk_array<Args>( chunk )... );

#ifndef NDEBUG
  ( assert( std::get<decltype( get_chunk_array<Args>( chunk ) )>( tuple ).ChunkOwner() == chunk ), ... );
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
  return query.With<Args...>();
}

} // namespace DeltaEngine::ECS_Internal

namespace DeltaEngine
{

//******************************************************************************
// EntityManager Public Methods
//******************************************************************************

inline EntityManager::EntityManager()
{
  auto *empty_arch = new Archetype();

  empty_arch->components = BuildComponentList( nullptr, 0 );
  empty_arch->owner = this;

  m_archetypes.push_back( empty_arch );
  m_archetypes_signature.push_back( 0 );

  CreateChunk( empty_arch );
}

inline EntityManager::~EntityManager()
{
  for ( Archetype *arch : m_archetypes )
  {
    for ( DataChunk *chunk : arch->chunks )
      delete chunk;
    delete arch->components;
    delete arch;
  }
}

template <typename... C>
EntityID EntityManager::CreateEntity()
{
  Archetype *arch { nullptr };

  if constexpr ( sizeof...( C ) != 0 )
  {
    static const Metatype *types[] = { Metatype::GetMetatype<C>()... };
    constexpr size_t count = ( sizeof( types ) / sizeof( *types ) );

    arch = FindOrCreateArchetype( types, count );
  }
  else
    arch = GetEmptyArchetype();

  EntityID id = AllocateEntity();
  SetEntityArchetype( id, arch );

  return id;
}

inline void EntityManager::DestroyEntity( EntityID id )
{
  ASSERT_ERROR( IsEntityValid( id ), "EntityManager: destroying invalid entity" );
  EraseEntityChunk( m_entities[id.index].chunk, m_entities[id.index].chunk_index );
  DeallocateEntity( id );
}

template <typename C>
bool EntityManager::HasComponent( EntityID id )
{
  Entity &ref = m_entities[id.index];
  auto c_array = ECS_Internal::get_chunk_array<C>( ref.chunk );
  return c_array.owner != nullptr;
}

template <typename C>
C &EntityManager::GetComponent( EntityID id )
{
  Entity &ref = m_entities[id.index];

  auto c_array = ECS_Internal::get_chunk_array<C>( ref.chunk );
  assert( c_array.ChunkOwner() != nullptr );
  return c_array[ref.chunk_index];
}

template <typename C>
void EntityManager::AddComponent( EntityID id, C &comp )
{
  AddComponent<C>( id );

  if ( !Metatype::GetMetatype<C>()->IsEmpty() )
    GetComponent<C>( id ) = comp;
}

template <typename C>
void EntityManager::AddComponent( EntityID id )
{
  const Metatype *temp_meta_array[32];
  const Metatype *type = Metatype::GetMetatype<C>();

  Archetype *arch = m_entities[id.index].chunk->header.owner;
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
    arch = FindOrCreateArchetype( temp_meta_array, ++count );
    SetEntityArchetype( id, arch );
  }
}

template <typename C>
void EntityManager::RemoveComponent( EntityID id )
{
  const Metatype *temp_meta_array[32] { nullptr };
  const Metatype *type = Metatype::GetMetatype<C>();

  Archetype *arch = m_entities[id.index].chunk->header.owner;
  ComponentList *list = arch->components;

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
    arch = FindOrCreateArchetype( *this, temp_meta_array, --count );
    SetEntityArchetype( arch, id );
  }
}

template <typename Func>
void EntityManager::ForEach( Func &&func )
{
  using params = decltype( ECS_Internal::args( &Func::operator() ) );

  Query query;
  ECS_Internal::unpack_query_with( params {}, query ).Build();

  ForEach<Func>( query, std::move( func ) );
}

template <typename Func>
void EntityManager::ForEach( Query &query, Func &&func )
{
  using params = decltype( ECS_Internal::args( &Func::operator() ) );

  ArchetypeIterate( query, [&]( Archetype *arch )
  {
    for ( auto chunk : arch->chunks )
      ECS_Internal::unpack_chunk( params {}, chunk, func );
  } );
}

inline Archetype *EntityManager::GetEmptyArchetype()
{
  return m_archetypes[0];
}

//******************************************************************************
// EntityManager Private Methods
//******************************************************************************

inline bool EntityManager::IsEntityValid( EntityID id )
{
  if ( m_entities.size() > id.index )
    if ( m_entities[id.index].generation == id.generation )
      return true;
  return false;
}

inline EntityID EntityManager::AllocateEntity()
{
  EntityID id;
  if ( m_entities_dead == 0 )
  {
    size_t index = m_entities.size();

    Entity new_entity;
    m_entities.push_back( new_entity );

    id.index = index;
  }
  else
  {
    size_t index = m_entities_deleted.back();
    m_entities_deleted.pop_back();
    id.generation = m_entities[index].generation;
    id.index = index;
    m_entities_dead--;
  }

  m_entities_live++;
  return id;
}

inline void EntityManager::DeallocateEntity( EntityID id )
{
  m_entities_deleted.push_back( id.index );
  m_entities[id.index].generation++;
  m_entities[id.index].chunk = nullptr;
  m_entities[id.index].chunk_index = 0;

  m_entities_live--;
  m_entities_dead++;
}

inline size_t EntityManager::InsertEntityChunk( DataChunk *chunk, EntityID id, bool initialize )
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

        if ( !type->IsEmpty() )
        {
          void *ptr = static_cast<void *>( reinterpret_cast<byte *>( chunk ) + ref.offset + ( type->size * index ) );
          type->constructor( ptr );
        }
      }
    }

    EntityID *id_ptr = reinterpret_cast<EntityID *>( chunk );
    id_ptr[index] = id;

    if ( chunk->header.index == list->capacity )
      SetChunkFull( chunk );
  }

  return index;
}

inline EntityID EntityManager::EraseEntityChunk( DataChunk *chunk, size_t index )
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

    if ( !type->IsEmpty() )
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
    DeleteChunk( chunk );
  else if ( full )
    SetChunkPartial( chunk );

  if ( pop )
  {
    m_entities[id_ptr[pop_index].index].chunk_index = index;
    id_ptr[index] = id_ptr[pop_index];
    return id_ptr[index];
  }

  return EntityID {};
}

inline void EntityManager::SetEntityArchetype( EntityID id, Archetype *arch )
{
  if ( !m_entities[id.index].chunk )
  {
    DataChunk *chunk = FindFreeChunk( arch );

    size_t index = InsertEntityChunk( chunk, id );
    m_entities[id.index].chunk = chunk;
    m_entities[id.index].chunk_index = index;
  }
  else
    MoveEntityToArchetype( id, arch, false );
}

inline void EntityManager::MoveEntityToArchetype( EntityID id, Archetype *arch, bool initialize )
{
  DataChunk *current_chunk = m_entities[id.index].chunk;
  DataChunk *target_chunk = FindFreeChunk( arch );

  size_t current_index = m_entities[id.index].chunk_index;
  size_t target_index = InsertEntityChunk( target_chunk, id, initialize );

  ComponentList *current_list = current_chunk->header.owner->components;
  ComponentList *target_list = target_chunk->header.owner->components;

  for ( size_t i = 0; i < current_list->metatypes.size(); i++ )
  {
    auto [current_type, current_offset] = current_list->metatypes[i];
    if ( !current_type->IsEmpty() )
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

  EraseEntityChunk( current_chunk, current_index );

  m_entities[id.index].chunk = target_chunk;
  m_entities[id.index].chunk_index = target_index;
}

inline Archetype *EntityManager::FindOrCreateArchetype( const Metatype **types, size_t count )
{
  assert( count < 32 );

  const size_t signature = Metatype::BuildSignature( types, count );

  for ( size_t i = 0; i < m_archetypes_signature.size(); i++ )
    if ( m_archetypes_signature[i] == signature )
    {
      ComponentList *list = m_archetypes[i]->components;
      if ( list->metatypes.size() == count )
      {
        for ( size_t j = 0; j < count; j++ )
          if ( list->metatypes[j].type != types[j] )
            break;

        return m_archetypes[i];
      }
    }

  // else

  Archetype *new_arch = new Archetype();
  new_arch->components = BuildComponentList( types, count );
  new_arch->component_hash = signature;
  new_arch->owner = this;

  m_archetypes.push_back( new_arch );
  m_archetypes_signature.push_back( signature );

  CreateChunk( new_arch );
  return new_arch;
}

template <typename Func>
void EntityManager::ArchetypeIterate( const Query &query, Func &&func )
{
  for ( size_t i = 0; i < m_archetypes_signature.size(); i++ )
  {
    if ( m_archetypes_signature[i] & query.m_required_matcher_hash )
    {
      ComponentList *list = m_archetypes[i]->components;
      if ( m_archetypes_signature[i] & query.m_excluded_matcher_hash )
      {
        bool skip { false };
        [&]()
        {
          for ( auto excluded : query.m_excluded )
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
      size_t match = std::count_if( query.m_required.begin(), query.m_required.end(), [&]( MetaHash hash )
      {
        for ( auto types : list->metatypes )
          if ( hash == types.type->hash )
            return true;
        return false;
      } );

      if ( match == query.m_required.size() )
        func( m_archetypes[i] );
    }
  }
}

inline ComponentList *EntityManager::BuildComponentList( const Metatype **types, size_t count )
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

inline void EntityManager::ReorderChunk( Archetype *arch )
{
  size_t size = arch->components->capacity;
  std::partition( arch->chunks.begin(), arch->chunks.end(), [size]( DataChunk *chnk )
  {
    return ( chnk->header.index == size );
  } );
}

inline void EntityManager::SetChunkFull( DataChunk *chunk )
{
  Archetype *arch = chunk->header.owner;
  arch->full_chunks++;

  ReorderChunk( arch );
}

inline void EntityManager::SetChunkPartial( DataChunk *chunk )
{
  Archetype *arch = chunk->header.owner;
  arch->full_chunks--;

  ReorderChunk( arch );
}

inline DataChunk *EntityManager::FindFreeChunk( Archetype *arch )
{
  DataChunk *chunk { nullptr };
  if ( arch->chunks.size() == 0 )
    chunk = CreateChunk( arch );
  else
  {
    chunk = arch->chunks[arch->chunks.size() - 1];
    if ( chunk->header.index == arch->components->capacity )
      chunk = CreateChunk( arch );
  }
  return chunk;
}

inline DataChunk *EntityManager::CreateChunk( Archetype *arch )
{
  DataChunk *chunk = new DataChunk();

  chunk->header.owner = arch;
  arch->chunks.push_back( chunk );
  return chunk;
}

inline void EntityManager::DeleteChunk( DataChunk *chunk )
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