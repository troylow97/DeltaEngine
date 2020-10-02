#pragma once
#include <vector>
#include "DE_API.h"
 
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
  };

} // namespace DeltaEngine