#include "Archetype.h"
#include "DataChunk.h"
#include "ComponentList.h"
#include <algorithm>
#include <vector>

namespace DeltaEngine
{

  void Archetype::reorder_chunk(Archetype *arch)
  {
    size_t size = arch->components->capacity;
    std::partition(arch->chunks.begin(), arch->chunks.end(), [size](DataChunk *chnk) {
      return (chnk->header.index == size);
    });
  }

  void Archetype::set_chunk_full(DataChunk *chunk)
  {
    Archetype *arch = chunk->header.owner;
    arch->full_chunks++;

    reorder_chunk(arch);
  }

  void Archetype::set_chunk_partial(DataChunk *chunk)
  {
    Archetype *arch = chunk->header.owner;
    arch->full_chunks--;

    reorder_chunk(arch);
  }

  DataChunk* Archetype::find_free_chunk(Archetype* arch)
  {
    DataChunk *chunk{nullptr};
    if ( arch->chunks.size() == 0)
      chunk = create_chunk(arch);
    else
    {
      chunk = arch->chunks[arch->chunks.size() - 1];
      if(chunk->header.index == arch->components->capacity)
        chunk = create_chunk(arch);
    }
    return chunk;
  }

  DataChunk *Archetype::create_chunk(Archetype *arch)
  {
    DataChunk *chunk = new DataChunk();

    chunk->header.owner = arch;
    arch->chunks.push_back(chunk);
    return chunk;
  }

  void Archetype::delete_chunk(DataChunk *chunk)
  {
    Archetype *owner = chunk->header.owner;
    DataChunk *backChunk = owner->chunks.back();

    if (backChunk != chunk)
      for (int i = 0; i < owner->chunks.size(); i++)
        if (owner->chunks[i] == chunk)
          owner->chunks[i] = backChunk;

    owner->chunks.pop_back();
    delete chunk;
  }
} // namespace DeltaEngine