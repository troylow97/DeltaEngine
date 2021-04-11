/**********************************************************************************
* \file   EntityManager.inl
* \brief  This file contains the implementation of EntityManager
*         The EntityManager is responsible managing  all entities within a world,
*         where component data are stored in chunks grouped according to their
*         archetype.
*
*         Each world is assigned a default EntityManager.
*
* \author Tan, Tong Wee, X% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Core/Debugging/Assert.h"
#include "Reflect/Reflect.h"
#include "ArrayView.h"

namespace DeltaEngine::ECS_Internal
{
template <typename... Ts>
struct ArgList
{};

template <typename T, typename Ret, typename... Args>
ArgList<Args...> args( Ret( T:: * )( Args ... ) const );

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
    constexpr ComponentMeta meta = ComponentMeta::Build<T_Base>();

    for ( auto comp : chunk->header.owner->components_desc->metalist )
      if ( comp.meta->bits & meta.bits )
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
void unpack_chunk( ArgList<Args...>, DataChunk *chunk, Func &&func )
{
  chunk_iterate<Args...>( chunk, func );
}

template <typename... Args>
Query &unpack_query_with( ArgList<Args...>, Query &query )
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
  Archetype *empty_arch = CreateEmptyArchetype();
  m_archetypes.push_back( empty_arch );
  CreateChunk( empty_arch );
}

inline EntityManager::~EntityManager()
{
  for ( Archetype *arch : m_archetypes )
    delete arch;
}

inline void EntityManager::Clear()
{
  for ( Archetype *arch : m_archetypes )
    delete arch;

  m_archetypes.clear();
  m_entities.clear();
  m_entities_deleted.clear();
  m_parents.clear();
  m_entities_live = 0;

  Archetype *empty_arch = CreateEmptyArchetype();
  m_archetypes.push_back( empty_arch );
  CreateChunk( empty_arch );
  ParticleEmitter::particlePools.clear();
}

template <typename... C>
EntityID EntityManager::CreateEntity()
{
  Archetype *arch { nullptr };

  if constexpr ( sizeof...( C ) != 0 )
  {
    static std::vector<const ComponentMeta *> meta_vec;
    if ( meta_vec.empty() )
    {
      ( meta_vec.push_back( ComponentMeta::GetComponentMeta<C>() ), ... );
      meta_vec.push_back( ComponentMeta::GetComponentMeta<EntityName>() );
      meta_vec.push_back( ComponentMeta::GetComponentMeta<EntityType>() );
      meta_vec.push_back( ComponentMeta::GetComponentMeta<Transform>() );
      auto last = std::unique( meta_vec.begin(), meta_vec.end() );
      meta_vec.erase( last, meta_vec.end() );
    }
    std::sort( meta_vec.begin(), meta_vec.end(), []( const ComponentMeta *lhs, const ComponentMeta *rhs )
    {
      return lhs->bits < rhs->bits;
    } );
    arch = FindOrCreateArchetype( meta_vec );
  }
  else
    arch = GetEmptyArchetype();

  EntityID id = AllocateEntity();
  SetEntityArchetype( id, arch );

  return id;
}

inline EntityID EntityManager::CreateEntityFromArchetype( EntityID id )
{
  EntityID new_id = AllocateEntity();
  auto &arch = m_entities[id.index].chunk->header.owner;
  SetEntityArchetype( new_id, arch );
  return new_id;
}

inline EntityID EntityManager::CloneEntity( EntityID id )
{
  EntityID new_id = AllocateEntity();
  CloneEntityArchetype( new_id, id );
  return new_id;
}

inline const std::vector<Entity> &EntityManager::GetEntities()
{
  return m_entities;
}


inline void EntityManager::DestroyEntity( EntityID id )
{
  ASSERT_ERROR( IsEntityValid( id ), "EntityManager: destroying invalid entity" )
    if ( HasComponent<Parent>( id ) )
    {
      auto p = GetComponent<Parent>( id );
      auto it = std::find_if( m_parents.begin(), m_parents.end(), [&]( ParentMap &pm )
      {
        return pm.first == p.p_id;
      } );
      if ( it != m_parents.end() )
        it->second.erase( std::remove_if( it->second.begin(), it->second.end(), [&]( size_t &cid )
      {
        return cid == id.index;
      } ), it->second.end() );
    }
  EraseEntityChunk( m_entities[id.index].chunk, m_entities[id.index].chunk_index );
  DeallocateEntity( id );
  DeltaEngine_CORE_INFO( "Entity - {} deleted", id.index );
}

template <typename C>
bool EntityManager::HasComponent( EntityID id )
{
  Entity &ref = m_entities[id.index];
  if ( ref.chunk )
    return ref.chunk->header.owner->bits_signature & ComponentMeta::GetComponentMeta<C>()->bits;
  return false;
}

template <typename C>
C &EntityManager::GetComponent( EntityID id )
{
  Entity &ref = m_entities[id.index];

  auto c_array = ECS_Internal::get_chunk_array<C>( ref.chunk );
  assert( c_array.ChunkOwner() != nullptr );
  return c_array[ref.chunk_index];
}

inline rttr::instance EntityManager::GetComponent( EntityID id, size_t bits )
{
  auto instance = RT_Reflect::RT_Getter( *this, id, bits );
  assert( instance.is_valid() );
  return instance;
}


template <typename C>
void EntityManager::AddComponent( EntityID id, C comp )
{
  if ( m_entities[id.index].chunk && !HasComponent<C>( id ) )
  {
    AddComponent<C>( id );

    if ( !ComponentMeta::GetComponentMeta<C>()->IsEmpty() )
      GetComponent<C>( id ) = std::move( comp );
  }
}

template <typename C>
void EntityManager::AddComponent( EntityID id )
{
  std::vector<const ComponentMeta *> meta_vec;
  const ComponentMeta *meta = ComponentMeta::GetComponentMeta<C>();

  Archetype *arch = m_entities[id.index].chunk->header.owner;
  Description *desc = arch->components_desc;

  for ( auto &details : desc->metalist )
    if ( !( details.meta->bits & meta->bits ) )
      meta_vec.push_back( details.meta );

  meta_vec.push_back( meta );

  std::sort( meta_vec.begin(), meta_vec.end(), []( const ComponentMeta *lhs, const ComponentMeta *rhs )
  {
    return lhs->bits < rhs->bits;
  } );

  arch = FindOrCreateArchetype( meta_vec );
  SetEntityArchetype( id, arch );
}

template <typename C>
void EntityManager::RemoveComponent( EntityID id )
{
  std::vector<const ComponentMeta *> meta_vec;
  const ComponentMeta *meta = ComponentMeta::GetComponentMeta<C>();

  Archetype *arch = m_entities[id.index].chunk->header.owner;

  if ( arch->bits_signature & meta->bits )
  {
    Description *desc = arch->components_desc;

    for ( auto &ref : desc->metalist )
      if ( !( ref.meta->bits & meta->bits ) )
        meta_vec.push_back( ref.meta );

    std::sort( meta_vec.begin(), meta_vec.end(), []( const ComponentMeta *lhs, const ComponentMeta *rhs )
    {
      return lhs->bits < rhs->bits;
    } );

    arch = FindOrCreateArchetype( meta_vec );
    SetEntityArchetype( id, arch );
  }
}

template <typename Func>
void EntityManager::ForEach( Func &&func )
{
  using params = decltype( ECS_Internal::args( &Func::operator() ) );

  Query arg_query;
  ECS_Internal::unpack_query_with( params {}, arg_query );

  ForEach<Func>( arg_query, std::move( func ) );
}

template <typename Func>
void EntityManager::ForEach( Query &query, Func &&func )
{
  using params = decltype( ECS_Internal::args( &Func::operator() ) );

  Query arg_query;
  ECS_Internal::unpack_query_with( params {}, arg_query );

  ArchetypeIterate( query + arg_query, [&]( Archetype *arch )
  {
    for ( auto chunk : arch->chunks )
      ECS_Internal::unpack_chunk( params {}, chunk, func );
  } );
}

inline Archetype *EntityManager::GetEmptyArchetype()
{
  return m_archetypes[0];
}

inline const Archetype *EntityManager::GetEntityArchetype( size_t id )
{
  if ( m_entities[id].chunk )
    return m_entities[id].chunk->header.owner;
  return nullptr;
}

inline void EntityManager::LinkParents()
{
  ForEach( [&]( EntityID &c, Parent &p )
  {
    SetParents( p.p_id, c.index );
  } );
}


inline void EntityManager::SetParents( size_t parent, size_t child )
{
  if ( !HasComponent<Parent>( { child } ) )
    AddComponent<Parent>( { child }, { parent } );
  else
  {
    auto &p = GetComponent<Parent>( { child } );
    auto it = std::find_if( m_parents.begin(), m_parents.end(), [&]( ParentMap &pm )
    {
      return pm.first == p.p_id;
    } );
    if ( it != m_parents.end() )
      it->second.erase( std::remove_if( it->second.begin(), it->second.end(), [&]( size_t &id )
    {
      return id == child;
    } ), it->second.end() );
    p.p_id = parent;
  }

  auto it = std::find_if( m_parents.begin(), m_parents.end(), [&]( ParentMap &pm )
  {
    return pm.first == parent;
  } );
  if ( it != m_parents.end() )
    it->second.push_back( child );
  else
    m_parents.push_back( { parent, {child} } );
}

inline std::vector<size_t> EntityManager::GetChildrens( size_t parent )
{
  for ( auto [p_id, c_vec] : m_parents )
    if ( p_id == parent )
      return c_vec;
  return {};
}


inline void EntityManager::CleanEntities()
{
  for ( auto it = m_parents.rbegin(); it < m_parents.rend(); ++it )
    if ( !IsEntityValid( { it->first } ) )
    {
      if ( it->first == u64_max )
        continue;
      DeltaEngine_CORE_INFO( "Parent: {}", it->first );
      while(!it->second.empty())
        DestroyEntity( { it->second[0]} );
      m_parents.erase( ( it + 1 ).base() );
    }
}


inline void EntityManager::TidyEntities()
{
  size_t end { 0 };
  bool flag { false };
  for ( size_t i = 0; i < m_entities.size(); i++ )
    if ( !IsEntityValid( { i } ) )
    {
      for ( size_t j = i + 1; j < m_entities.size(); j++ )
        if ( IsEntityValid( { j } ) )
        {
          if ( HasComponent<Parent>( { j } ) )
          {
            auto it = std::find_if( m_parents.begin(), m_parents.end(), [&]( ParentMap &pm )
            {
              return pm.first == GetComponent<Parent>( { j } ).p_id;
            } );
            if ( it != m_parents.end() )
              for ( auto &child : it->second )
                if ( child == j )
                  child = i;
          }
          else
          {
            auto it = std::find_if( m_parents.begin(), m_parents.end(), [&]( ParentMap &pm )
            {
              return pm.first == j;
            } );
            if ( it != m_parents.end() )
            {
              for ( auto &child : it->second )
                GetComponent<Parent>( { child } ).p_id = i;
              it->first = i;
            }
          }

          reinterpret_cast<EntityID *>( m_entities[j].chunk )[m_entities[j].chunk_index].index = i;

          std::swap( m_entities[i], m_entities[j] );
          flag = true;
          break;
        }
      if ( !flag )
      {
        end = i;
        break;
      }
      flag = false;
    }

  m_entities.erase( m_entities.begin() + end, m_entities.end() );
  m_entities.shrink_to_fit();
  m_entities_deleted.clear();
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
  if ( m_entities_deleted.empty() )
  {
    id.index = m_entities.size();
    m_entities.emplace_back();
  }
  else
  {
    id.index = m_entities_deleted.back();
    m_entities_deleted.pop_back();
    id.generation = m_entities[id.index].generation;
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
}

inline size_t EntityManager::InsertEntityChunk( DataChunk *chunk, EntityID id, bool initialize )
{
  size_t index { 0 };

  Description *desc = chunk->header.owner->components_desc;

  if ( chunk->header.index < desc->capacity )
  {
    index = chunk->header.index++;

    if ( initialize )
    {
      for ( auto &ref : desc->metalist )
      {
        const ComponentMeta *type = ref.meta;

        if ( !type->IsEmpty() )
        {
          void *ptr = static_cast<void *>( reinterpret_cast<byte *>( chunk ) + ref.offset + ( type->size * index ) );
          type->constructor( ptr );
        }
      }
    }

    EntityID *id_ptr = reinterpret_cast<EntityID *>( chunk );
    id_ptr[index] = id;

    if ( chunk->header.index == desc->capacity )
      SetChunkFull( chunk );
  }

  return index;
}

inline void EntityManager::EraseEntityChunk( DataChunk *chunk, size_t index )
{
  assert( chunk->header.index >= index );

  Description *desc = chunk->header.owner->components_desc;

  bool full = chunk->header.index == desc->capacity;
  bool pop = chunk->header.index > 1 && index != ( chunk->header.index - 1 );

  size_t pop_index = chunk->header.index - 1;


  for ( const auto &[type, offset] : desc->metalist )
    if ( !type->IsEmpty() )
    {
      void *ptr = static_cast<void *>(
        reinterpret_cast<byte *>( chunk ) + offset + ( type->size * index ) );
      type->destructor( ptr );

      if ( pop )
      {
        void *pop_ptr = static_cast<void *>(
          reinterpret_cast<byte *>( chunk ) + offset + ( type->size * pop_index ) );
        std::memcpy( ptr, pop_ptr, type->size );

        switch ( type->bits )
        {
          case ComponentMeta::ComponentBits<Animator>() :
          {
            *static_cast<Animator *>( ptr ) = *static_cast<Animator *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<State>() :
          {
            *static_cast<State *>( ptr ) = *static_cast<State *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<Image>() :
          {
            *static_cast<Image *>( ptr ) = *static_cast<Image *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<Renderer2D>() :
          {
            *static_cast<Renderer2D *>( ptr ) = *static_cast<Renderer2D *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<RendererOverlay>() :
          {
            std::swap( *static_cast<RendererOverlay *>( ptr ), *static_cast<RendererOverlay *>( pop_ptr ) );
            break;
          }
          case ComponentMeta::ComponentBits<EntityName>() :
          {
            *static_cast<EntityName *>( ptr ) = *static_cast<EntityName *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<EntityType>() :
          {
            *static_cast<EntityType *>( ptr ) = *static_cast<EntityType *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<AI>() :
          {
            *static_cast<AI *>( ptr ) = *static_cast<AI *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<Text>() :
          {
            *static_cast<Text *>( ptr ) = *static_cast<Text *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<Button>() :
          {
            *static_cast<Button *>( ptr ) = *static_cast<Button *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<Toggle>() :
          {
            *static_cast<Toggle *>( ptr ) = *static_cast<Toggle *>( pop_ptr );
            break;
          }
          case ComponentMeta::ComponentBits<AudioSource>() :
          {
            *static_cast<AudioSource *>( ptr ) = *static_cast<AudioSource *>( pop_ptr );
            break;
          }
          default:
            std::memcpy( ptr, pop_ptr, type->size );
        }
      }
    }

  EntityID *id_ptr = reinterpret_cast<EntityID *>( chunk );
  id_ptr[index] = EntityID {};

  chunk->header.index--;

  if ( chunk->header.index == 0 )
    DeleteChunk( chunk );
  else if ( full )
    SetChunkPartial( chunk );

  if ( pop )
  {
    m_entities[id_ptr[pop_index].index].chunk_index = index;
    id_ptr[index] = id_ptr[pop_index];
  }
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
    MoveEntityToArchetype( id, arch );
}

inline void EntityManager::CloneEntityArchetype( EntityID new_id, EntityID id )
{
  Archetype *arch = m_entities[id.index].chunk->header.owner;
  SetEntityArchetype( new_id, arch );

  DataChunk *current_chunk = m_entities[id.index].chunk;
  DataChunk *new_chunk = m_entities[new_id.index].chunk;

  size_t current_index = m_entities[id.index].chunk_index;
  size_t new_index = m_entities[new_id.index].chunk_index;

  for ( auto &[type, offset] : arch->components_desc->metalist )
    if ( !type->IsEmpty() )
    {
      void *current = static_cast<void *>(
        reinterpret_cast<byte *>( current_chunk ) +
        offset +
        ( type->size * current_index ) );

      void *target = static_cast<void *>(
        reinterpret_cast<byte *>( new_chunk ) +
        offset +
        ( type->size * new_index ) );

      switch ( type->bits )
      {
        case ComponentMeta::ComponentBits<Animator>() :
        {
          *static_cast<Animator *>( target ) = *static_cast<Animator *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<State>() :
        {
          *static_cast<State *>( target ) = *static_cast<State *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<Image>() :
        {
          *static_cast<Image *>( target ) = *static_cast<Image *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<Renderer2D>() :
        {
          *static_cast<Renderer2D *>( target ) = *static_cast<Renderer2D *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<RendererOverlay>() :
        {
          std::swap( *static_cast<RendererOverlay *>( current ), *static_cast<RendererOverlay *>( target ) );
          break;
        }
        case ComponentMeta::ComponentBits<EntityName>() :
        {
          *static_cast<EntityName *>( target ) = *static_cast<EntityName *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<EntityType>() :
        {
          *static_cast<EntityType *>( target ) = *static_cast<EntityType *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<AI>() :
        {
          *static_cast<AI *>( target ) = *static_cast<AI *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<Text>() :
        {
          *static_cast<Text *>( target ) = *static_cast<Text *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<Button>() :
        {
          *static_cast<Button *>( target ) = *static_cast<Button *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<Toggle>() :
        {
          *static_cast<Toggle *>( target ) = *static_cast<Toggle *>( current );
          break;
        }
        case ComponentMeta::ComponentBits<AudioSource>() :
        {
          *static_cast<AudioSource *>( target ) = *static_cast<AudioSource *>( current );
          break;
        }
        default:
          std::memcpy( target, current, type->size );
      }
    }
}


inline void EntityManager::MoveEntityToArchetype( EntityID id, Archetype *arch )
{
  DataChunk *current_chunk = m_entities[id.index].chunk;
  DataChunk *target_chunk = FindFreeChunk( arch );

  size_t current_index = m_entities[id.index].chunk_index;
  size_t target_index = InsertEntityChunk( target_chunk, id, true );

  Description *current_desc = current_chunk->header.owner->components_desc;
  Description *target_desc = target_chunk->header.owner->components_desc;

  for ( auto &[current_type, current_offset] : current_desc->metalist )
    if ( !current_type->IsEmpty() )
      for ( auto &[target_type, target_offset] : target_desc->metalist )
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

          switch ( current_type->bits )
          {
            case ComponentMeta::ComponentBits<Animator>() :
            {
              std::swap( *static_cast<Animator *>( current ), *static_cast<Animator *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<State>() :
            {
              std::swap( *static_cast<State *>( current ), *static_cast<State *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<Image>() :
            {
              std::swap( *static_cast<Image *>( current ), *static_cast<Image *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<Renderer2D>() :
            {
              std::swap( *static_cast<Renderer2D *>( current ), *static_cast<Renderer2D *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<RendererOverlay>() :
            {
              std::swap( *static_cast<RendererOverlay *>( current ), *static_cast<RendererOverlay *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<EntityName>() :
            {
              std::swap( *static_cast<EntityName *>( current ), *static_cast<EntityName *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<EntityType>() :
            {
              std::swap( *static_cast<EntityType *>( current ), *static_cast<EntityType *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<AI>() :
            {
              std::swap( *static_cast<AI *>( current ), *static_cast<AI *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<Text>() :
            {
              std::swap( *static_cast<Text *>( current ), *static_cast<Text *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<Button>() :
            {
              std::swap( *static_cast<Button *>( current ), *static_cast<Button *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<Toggle>() :
            {
              std::swap( *static_cast<Toggle *>( current ), *static_cast<Toggle *>( target ) );
              break;
            }
            case ComponentMeta::ComponentBits<AudioSource>() :
            {
              std::swap( *static_cast<AudioSource *>( current ), *static_cast<AudioSource *>( target ) );
              break;
            }
            default:
              std::memcpy( target, current, current_type->size );
          }
        }
  EraseEntityChunk( current_chunk, current_index );

  m_entities[id.index].chunk = target_chunk;
  m_entities[id.index].chunk_index = target_index;
}

inline Archetype *EntityManager::CreateEmptyArchetype()
{
  auto *empty_arch = new Archetype();
  std::vector<const ComponentMeta *> empty_vec;
  empty_vec.push_back( ComponentMeta::GetComponentMeta<EntityName>() );
  empty_vec.push_back( ComponentMeta::GetComponentMeta<EntityType>() );
  empty_vec.push_back( ComponentMeta::GetComponentMeta<Transform>() );
  std::sort( empty_vec.begin(), empty_vec.end(), []( const ComponentMeta *lhs, const ComponentMeta *rhs )
  {
    return lhs->bits < rhs->bits;
  } );
  empty_arch->components_desc = BuildDescription( empty_vec );
  empty_arch->bits_signature = ComponentMeta::BuildSignature( empty_vec );
  empty_arch->owner = this;
  return empty_arch;
}

inline Archetype *EntityManager::FindOrCreateArchetype( const std::vector<const ComponentMeta *> &meta_vec )
{
  ASSERT_ERROR( meta_vec.size() <= 64, "Support up to 64 components_desc only" );

  const size_t signature = ComponentMeta::BuildSignature( meta_vec );

  for ( auto &archetype : m_archetypes )
    if ( archetype->bits_signature == signature )
      return archetype;

  // else

  Archetype *new_arch = new Archetype();
  new_arch->components_desc = BuildDescription( meta_vec );
  new_arch->bits_signature = signature;
  new_arch->owner = this;

  m_archetypes.push_back( new_arch );

  CreateChunk( new_arch );
  return new_arch;
}

template <typename Func>
void EntityManager::ArchetypeIterate( const Query &query, Func &&func )
{
  for ( auto &archetype : m_archetypes )
    if ( !( archetype->bits_signature & query.m_excluded_bits ) )
      if ( !( ~archetype->bits_signature & query.m_required_bits ) )
        func( archetype );
}

inline Description *EntityManager::BuildDescription( const std::vector<const ComponentMeta *> &meta_vec )
{
  Description *desc = new Description();

  size_t total_size = sizeof( EntityID );
  for ( auto &meta : meta_vec )
    total_size += meta->size;

  size_t storage_space = sizeof( DataChunk::data );

  size_t slots = ( storage_space / total_size ) - 1;

  size_t offset { 0 };
  offset += sizeof( EntityID ) * slots;

  for ( auto &meta : meta_vec )
  {
    if ( meta->align != 0 )
    {
      size_t r = offset % meta->align;
      size_t type_offset = meta->align - r;
      offset += type_offset;
    }

    desc->metalist.push_back( { meta, offset } );

    if ( meta->align != 0 )
      offset += meta->size * slots;
  }

  assert( offset <= MEMORY_BLOCK_16K );

  desc->capacity = slots;

  return desc;
}

inline void EntityManager::ReorderChunk( Archetype *arch )
{
  size_t size = arch->components_desc->capacity;
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
  if ( arch->chunks.empty() )
    chunk = CreateChunk( arch );
  else
  {
    chunk = arch->chunks[arch->chunks.size() - 1];
    if ( chunk->header.index == arch->components_desc->capacity )
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
    for ( auto &ref : owner->chunks )
      if ( ref == chunk )
        ref = backChunk;

  owner->chunks.pop_back();
  delete chunk;
}
} // namespace DeltaEngine
