#pragma once

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cassert>
#include <tuple>

#include "Metatype.h"
#include "Query.h"
#include "Entities.h"
#include "Archetype.h"
#include "ComponentList.h"
#include "ArrayView.h"
#include "DataChunk.h"

namespace DeltaEngine
{

  class EntityManager
  {
    std::vector<Entity> entities;
    std::vector<size_t> entities_deleted;

    std::vector<size_t> archetypes_signature;
    std::vector<Archetype *> archetypes;

    size_t entities_live{0};
    size_t entities_dead{0};

  public:
    EntityManager();

    ~EntityManager();

    template <typename... C>
    EntityID create_entity();

    void destroy_entity(EntityID id);

    template <typename C>
    bool has_component(EntityID id);

    template <typename C>
    C &get_component(EntityID id);

    template <typename C>
    void add_component(EntityID id, C &comp);

    template <typename C>
    void add_component(EntityID id);

    template <typename C>
    void remove_component(EntityID id);

    template <typename Func>
    void for_each(Func &&function);

    template <typename Func>
    void for_each(Query &query, Func &&function);

    Archetype *get_empty_archetype();

  private:
    bool is_entity_valid(EntityID id);

    EntityID allocate_entity();

    void deallocate_entity(EntityID id);

    size_t insert_entity_chunk(DataChunk *chunk, EntityID id, bool initialize = true);

    EntityID erase_entity_chunk(DataChunk *chunk, size_t index);

    void set_entity_archetype(EntityID id, Archetype *arch);

    void move_entity_to_archetype(EntityID id, Archetype *arch, bool initialize = true);

    Archetype *find_or_create_archetype(const Metatype **types, size_t count);

    template <typename Func>
    void archetype_iterate(const Query &query, Func &&func);

    ComponentList *build_component_list(const Metatype **types, size_t count);
  };

} // namespace DeltaEngine

#include "EntityManager.inl"