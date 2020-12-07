#pragma once

#include <vector>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>


#include "ComponentMeta.h"
#include "Query.h"
#include "Entities.h"
#include "Archetype.h"
#include "Description.h"
#include "DataChunk.h"

namespace DeltaEngine
{
  namespace Serialize
  {
    void WriteEntities(class EntityManager& em, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);
  }

  class EntityManager
  {
    std::vector<Entity> m_entities;
    std::vector<size_t> m_entities_deleted;
    std::vector<Archetype*> m_archetypes;

    size_t m_entities_live{0};

    friend class World;
    friend void Serialize::WriteEntities(class EntityManager& em,
                                         rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);
  public:
    EntityManager();

    ~EntityManager();

    void Clear();

    template <typename... C>
    EntityID CreateEntity();

    EntityID CreateEntityFromArchetype(EntityID id);

    EntityID CloneEntity(EntityID id);

    const std::vector<Entity>& GetEntities();

    void DestroyEntity(EntityID id);

    template <typename C>
    bool HasComponent(EntityID id);

    template <typename C>
    C& GetComponent(EntityID id);

    rttr::instance GetComponent(EntityID id, size_t bits);

    template <typename C>
    void AddComponent(EntityID id, C comp);

    template <typename C>
    void AddComponent(EntityID id);

    template <typename C>
    void RemoveComponent(EntityID id);

    template <typename Func>
    void ForEach(Func&& function);

    template <typename Func>
    void ForEach(Query& query, Func&& function);

    Archetype* GetEmptyArchetype();

    const Archetype* GetEntityArchetype(size_t id);

    bool IsEntityValid(EntityID id);
  private:

    EntityID AllocateEntity();

    void DeallocateEntity(EntityID id);

    size_t InsertEntityChunk(DataChunk* chunk, EntityID id, bool initialize = true);

    void EraseEntityChunk(DataChunk* chunk, size_t index);

    void SetEntityArchetype(EntityID id, Archetype* arch);

    void CloneEntityArchetype(EntityID new_id, EntityID id);

    void MoveEntityToArchetype(EntityID id, Archetype* arch);

    Archetype* CreateEmptyArchetype();

    Archetype* FindOrCreateArchetype(const std::vector<const ComponentMeta*>& meta_vec);

    template <typename Func>
    void ArchetypeIterate(const Query& query, Func&& func);

    Description* BuildDescription(const std::vector<const ComponentMeta*>& meta_vec);

    void ReorderChunk(Archetype* arch);
    void SetChunkFull(DataChunk* chunk);
    void SetChunkPartial(DataChunk* chunk);
    DataChunk* FindFreeChunk(Archetype* arch);
    DataChunk* CreateChunk(Archetype* arch);
    void DeleteChunk(DataChunk* chunk);
  };
} // namespace DeltaEngine

#include "EntityManager.inl"
