/**********************************************************************************
* \file   Entities.h
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

namespace DeltaEngine
{
  struct DataChunk;

  struct EntityID
  {
      size_t index{ 0 };
      size_t generation{ 1 };

      bool operator==(const EntityID& rhs) const
      {
          return generation == rhs.generation &&
              index == rhs.index;
      }
  };

  struct Entity
  {
    DataChunk* chunk{nullptr};
    size_t chunk_index{0};
    size_t generation{1};

    bool operator==(const Entity& rhs) const
    {
      return chunk == rhs.chunk &&
        generation == rhs.generation &&
        chunk_index == rhs.chunk_index;
    }

    bool operator!=(const Entity& rhs) const
    {
      return !(*this == rhs);
    }
  };
} // namespace DeltaEngine
