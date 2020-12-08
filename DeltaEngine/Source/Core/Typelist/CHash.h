/**********************************************************************************
* \file   CHash.h
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
#include <cstdint>

namespace DeltaEngine
{
  constexpr uint64_t hash_fnv1a(const char* key)
  {
    uint64_t hash = 0xcbf29ce484222325;
    const uint64_t prime = 0x100000001b3;

    int i = 0;
    while (key[i])
    {
      const uint8_t value = key[i++];
      hash = hash ^ value;
      hash *= prime;
    }

    return hash;
  }

  struct CHash
  {
    uint64_t digest{0};

    bool operator==(const CHash& rhs) const
    {
      return digest == rhs.digest;
    }

    template <typename T>
    static constexpr CHash Hash()
    {
      return CHash{hash_fnv1a(__FUNCSIG__)};
    }
  };
} // namespace DeltaEngine
