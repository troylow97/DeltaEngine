#pragma once
#include "DE_API.h"

namespace DeltaEngine
{

  using byte = unsigned char;
  constexpr size_t MEMORY_BLOCK_16K = 16384;

  struct ComponentList;
  struct Archetype;
  struct EntityID;

  struct DataChunkHeader
  {
    Archetype *owner{nullptr};
    size_t index{0};
  };

  struct alignas(64) DataChunk
  {
    byte data[MEMORY_BLOCK_16K - sizeof(DataChunkHeader)];
    DataChunkHeader header;
  };

  static_assert(sizeof(DataChunk) == MEMORY_BLOCK_16K);
} // namespace DeltaEngine