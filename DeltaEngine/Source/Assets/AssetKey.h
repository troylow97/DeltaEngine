/**********************************************************************************
* \file   AssetKey.h
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
#include <string_view>
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
  class AssetKey
  {
  public:

    AssetKey() = default;

    explicit AssetKey(const size_t digest) :
      m_digest(digest)
    {
    }

    AssetKey(const std::string& str) :
      m_digest(std::hash<std::string>{}(str)), m_key{str}
    {
    }

    template <size_t Size>
    constexpr AssetKey(const char ( &str )[Size]) :
      m_digest(std::hash<std::string>{}(std::string(str))), m_key{str}
    {
    }

    bool operator==(const AssetKey& rhs) const
    {
      return m_digest == rhs.m_digest;
    }

    size_t operator()() const
    {
      return m_digest;
    }

    const std::string& Key() const
    {
      return m_key;
    }

  private:
    size_t m_digest{0};
    std::string m_key{};
  };
} // namespace DeltaEngine

namespace std
{
  template <>
  struct hash<DeltaEngine::AssetKey>
  {
    std::size_t operator()(const DeltaEngine::AssetKey key) const noexcept
    {
      return key();
    }
  };
} // namespace std
