#pragma once

#include <vector>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>


#include "Metatype.h"
#include "Query.h"
#include "Entities.h"
#include "Archetype.h"
#include "ComponentList.h"
#include "ArrayView.h"
#include "DataChunk.h"
#include "DE_API.h"

namespace DeltaEngine
{

  namespace Serialize
  {
    void WriteEntities( class DeltaEngine::EntityManager &em, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer );
  }

  class DE_API EntityManager
  {
    std::vector<Entity> m_entities;
    std::vector<size_t> m_entities_deleted;

    std::vector<size_t> m_archetypes_signature;
    std::vector<Archetype *> m_archetypes;

    size_t m_entities_live{0};
    size_t m_entities_dead{0};
    friend class World;
    friend void DeltaEngine::Serialize::WriteEntities( class DeltaEngine::EntityManager &em, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer );
  public:
    EntityManager();

    ~EntityManager();

    template <typename... C>
    EntityID CreateEntity();

    void DestroyEntity(EntityID id);

    template <typename C>
    bool HasComponent(EntityID id);

    template <typename C>
    C &GetComponent(EntityID id);

    template <typename C>
    void AddComponent(EntityID id, C &comp);

    template <typename C>
    void AddComponent(EntityID id);

    template <typename C>
    void RemoveComponent(EntityID id);

    template <typename Func>
    void ForEach(Func &&function);

    template <typename Func>
    void ForEach(Query &query, Func &&function);

    Archetype *GetEmptyArchetype();

  private:
    bool IsEntityValid(EntityID id);

    EntityID AllocateEntity();

    void DeallocateEntity(EntityID id);

    size_t InsertEntityChunk(DataChunk *chunk, EntityID id, bool initialize = true);

    EntityID EraseEntityChunk(DataChunk *chunk, size_t index);

    void SetEntityArchetype(EntityID id, Archetype *arch);

    void MoveEntityToArchetype(EntityID id, Archetype *arch, bool initialize = true);

    Archetype *FindOrCreateArchetype(const Metatype **types, size_t count);

    template <typename Func>
    void ArchetypeIterate(const Query &query, Func &&func);

    ComponentList *BuildComponentList(const Metatype **types, size_t count);

    void ReorderChunk(Archetype *arch);
    void SetChunkFull(DataChunk *chunk);
    void SetChunkPartial(DataChunk *chunk);
    DataChunk *FindFreeChunk(Archetype *arch);
    DataChunk *CreateChunk(Archetype *arch);
    void DeleteChunk(DataChunk *chunk);
  };

} // namespace DeltaEngine

#include "EntityManager.inl"