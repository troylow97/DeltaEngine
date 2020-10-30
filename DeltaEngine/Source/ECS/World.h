#pragma once
#include "Core/Utils/Json/JsonFile.h"
#include "EntityManager.h"
#include "SystemBase.h"
#include "Core/Typelist/CHash.h"
#include <memory>

namespace DeltaEngine
{

class DE_API World
{
#pragma warning(disable:4251)
  std::unique_ptr<EntityManager> em;
  std::unordered_map<size_t, std::unique_ptr<SystemBase>> systems;
  std::vector<size_t> update_sequence;
  std::vector<size_t> late_update_sequence;
#pragma warning(default:4251)

  bool system_exist( size_t digest )
  {
    if ( systems.find( digest ) == systems.end() )
      return false;
    return true;
  }


public:
  World() : em( std::make_unique<EntityManager>() )
  {}

  EntityManager &get_entity_manager() const
  {
    return *em;
  }

  template <typename... Systems>
  void create_systems()
  {
    ( find_or_create_system<Systems>(), ... );
  }

  template <typename System,
    typename = std::enable_if<std::is_base_of_v<SystemBase, System>>>
    SystemBase &find_or_create_system()
  {
    constexpr CHash hash = CHash::Hash<System>();
    const auto it = systems.find( hash.digest );
    if ( it == systems.end() )
    {
      systems[hash.digest] = std::make_unique<System>( *em );
      return *( systems[hash.digest] );
    }
    return *( it->second );
  }

  void update()
  {
    for ( auto hash : update_sequence )
      systems[hash]->Update();
  }

  void late_update()
  {
    for ( auto hash : late_update_sequence )
      systems[hash]->LateUpdate();
  }

  template <typename... Systems>
  void set_update_sequence()
  {
    update_sequence.clear();
    std::vector<CHash> vec_hash = { CHash::Hash<Systems>()... };
    for ( auto hash : vec_hash )
    {
      assert( system_exist( hash.digest ) );
      update_sequence.push_back( hash.digest );
    }
  }

  template <typename... Systems>
  void set_late_update_sequence()
  {
    late_update_sequence.clear();
    std::vector<CHash> vec_hash = { CHash::Hash<Systems>()... };
    for ( auto hash : vec_hash )
    {
      assert( system_exist( hash.digest ) );
      late_update_sequence.push_back( hash.digest );
    }
  }

  void Save( std::string filename )
  {
    JsonFile file;
    file.StartWriter( filename ).WriteEntities( *em ).EndWriter();
  }

  void Load( std::string filename )
  {
    JsonFile file;
    file.StartReader( filename ).LoadEntities( *em ).EndReader();
  }
};

}// namespace DeltaEngine