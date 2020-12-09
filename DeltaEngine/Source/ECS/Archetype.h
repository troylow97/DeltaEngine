/**********************************************************************************
* \file   Archetype.h
* \brief  This file contains the definition for Archetype
*         Each Archetype is:
*         - Owned by an EntityManager
*         - Contains a description that details the type in this Archetype
*         - A unique bits_signature based on the type of components
*         - A vector of data chunk where the actual data is stored
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
      for (auto& [type, offset] : components_desc->metalist)
        if (!type->IsEmpty())
          for (auto& chunk : chunks)
            for (size_t i = 0; i < chunk->header.index; i++)
            {
              void* ptr = static_cast<void*>(
                reinterpret_cast<byte*>(chunk) + offset + (type->size * i));
              type->destructor(ptr);
            }

      for (auto chunk : chunks)
        delete chunk;
      chunks.clear();
      delete components_desc;
    }
  };
} // namespace DeltaEngine
