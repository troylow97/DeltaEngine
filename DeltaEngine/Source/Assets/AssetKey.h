#pragma once
#include <string_view>

namespace DeltaEngine
{
  class AssetKey
  {
  public:

    AssetKey() = default;

    explicit AssetKey(const size_t digest) :
      _digest(digest)
    {
    }

    AssetKey(const std::string_view str) :
      _digest(std::hash<std::string_view>{}(str))
    {
    }

    template <size_t Size>
    constexpr AssetKey(const char ( &str )[Size]) :
      _digest(std::hash<std::string_view>{}(std::string_view(str)))
    {
    }

    bool operator==(const AssetKey& rhs) const
    {
      return _digest == rhs._digest;
    }

    size_t operator()() const
    {
      return _digest;
    }

  private:
    size_t _digest{0};
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
