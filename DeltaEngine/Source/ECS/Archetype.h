/**********************************************************************************
* \file   Archetype.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
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
  EntityManager *owner { nullptr };
  Description *components_desc { nullptr };
  size_t bits_signature { 0 };
  size_t full_chunks { 0 }; // For performance checking, not implemented as of yet
  std::vector<DataChunk *> chunks;

  //~Archetype()
  //{
  //  for (auto chunk : chunks)
  //    delete chunk;
  //  chunks.clear();
  //  delete components_desc;
  //}

  ~Archetype()
  {
    for ( auto &[type, offset] : components_desc->metalist )
      if ( !type->IsEmpty() )
        for ( auto &chunk : chunks )
          for ( size_t i = 0; i < chunk->header.index; i++ )
          {
            void *ptr = static_cast<void *>(
              reinterpret_cast<byte *>( chunk ) + offset + ( type->size * i ) );
            type->destructor( ptr );
          }

      for (auto chunk : chunks)
        delete chunk;
      chunks.clear();
      delete components_desc;
  }
};
} // namespace DeltaEngine
