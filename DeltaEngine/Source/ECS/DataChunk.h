/**********************************************************************************
* \file   DataChunk.h
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
  using byte = unsigned char;
  constexpr size_t MEMORY_BLOCK_16K = 16384;

  struct Archetype;

  struct DataChunkHeader
  {
    Archetype* owner{nullptr};
    size_t index{0};
  };

  struct alignas( 64 ) DataChunk
  {
    byte data[MEMORY_BLOCK_16K - sizeof(DataChunkHeader)]{0};
    DataChunkHeader header;
  };

  static_assert( sizeof(DataChunk) == MEMORY_BLOCK_16K );
} // namespace DeltaEngine
