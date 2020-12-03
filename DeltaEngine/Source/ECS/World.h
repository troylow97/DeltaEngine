#pragma once
#include "Core/Utils/Json/JsonFile.h"
#include "EntityManager.h"
#include "SystemBase.h"
#include "Core/Typelist/CHash.h"
#include <memory>

namespace DeltaEngine
{
  class World
  {
    std::unique_ptr<EntityManager> em;
    std::unordered_map<size_t, std::unique_ptr<SystemBase>> systems;
    std::vector<size_t> update_sequence;
    std::vector<size_t> late_update_sequence;
    bool m_pause{true};


    bool SystemExist(size_t digest);

  public:
    World();

    void SetPause(bool pause);

    EntityManager& GetEntityManager() const;

    template <typename... Systems>
    void CreateSystems()
    {
      ( FindOrCreateSystem<Systems>(), ... );
    }

    template <typename System,
              typename = std::enable_if<std::is_base_of_v<SystemBase, System>>>
    SystemBase& FindOrCreateSystem()
    {
      constexpr CHash hash = CHash::Hash<System>();
      const auto it = systems.find(hash.digest);
      if (it == systems.end())
      {

        systems[hash.digest] = std::make_unique<System>(*em);
        return *(systems[hash.digest]);
      }
      return *(it->second);
    }

    void InitSystems();

    void ShutdownSystems();

    void Run();

    void Update();

    void LateUpdate();

    template <typename... Systems>
    void SetUpdateSequence()
    {
      update_sequence.clear();
      std::vector<CHash> vec_hash = {CHash::Hash<Systems>()...};
      for (auto hash : vec_hash)
      {
        assert(SystemExist( hash.digest ));
        update_sequence.push_back(hash.digest);
      }
    }

    template <typename... Systems>
    void SetLateUpdateSequence()
    {
      late_update_sequence.clear();
      std::vector<CHash> vec_hash = {CHash::Hash<Systems>()...};
      for (auto hash : vec_hash)
      {
        assert(SystemExist( hash.digest ));
        late_update_sequence.push_back(hash.digest);
      }
    }

    void Save(std::string filename);

    void Load(std::string filename);
  };
} // namespace DeltaEngine
