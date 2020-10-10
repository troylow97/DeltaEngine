#pragma once
#include <vector>
#include "DE_API.h"
 
namespace DeltaEngine
{
  struct ComponentList;
  class EntityManager;
  struct DataChunk;

  struct Archetype
  {
    ComponentList *components{nullptr};
    EntityManager *owner{nullptr};
    size_t component_hash{0};
    size_t full_chunks{0};
    std::vector<DataChunk *> chunks;
  };

} // namespace DeltaEngine