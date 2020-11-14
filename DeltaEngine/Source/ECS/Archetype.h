#pragma once
#include <vector>
#include "ECS/DataChunk.h"
#include "ECS/Description.h"

namespace DeltaEngine
{
  class EntityManager;
  struct Description;
  struct DataChunk;

  struct Archetype
  {
    EntityManager* owner{nullptr};
    Description* components_desc{nullptr};
    size_t bits_signature{0};
    size_t full_chunks{0}; // For performance checking, not implemented as of yet
    std::vector<DataChunk*> chunks;

    ~Archetype()
    {
      delete components_desc;
      for (auto chunk : chunks)
        delete chunk;
      chunks.clear();
    }
  };
} // namespace DeltaEngine
