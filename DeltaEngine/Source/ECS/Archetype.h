#pragma once
#include <vector>



namespace DeltaEngine
{
  struct ComponentList;
  class EntityManager;
  struct DataChunk;

namespace ECS_Internal
{
  template <typename T>
  auto get_chunk_array(DataChunk *chunk);
}

  class Archetype
  {
    ComponentList *components{nullptr};
    EntityManager *owner{nullptr};
    size_t component_hash{0};
    size_t full_chunks{0};
    std::vector<DataChunk *> chunks;
    friend class EntityManager;

    template <typename T>
    friend auto ECS_Internal::get_chunk_array(DataChunk *chunk);

  public:
    static void reorder_chunk(Archetype *arch);
    static void set_chunk_full(DataChunk *chunk);
    static void set_chunk_partial(DataChunk *chunk);

    static DataChunk *find_free_chunk(Archetype *arch);
    static DataChunk *create_chunk(Archetype *arch);
    static void delete_chunk(DataChunk *chunk);
  };

} // namespace DeltaEngine