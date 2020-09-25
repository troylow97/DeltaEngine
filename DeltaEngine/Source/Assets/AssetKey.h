#pragma once
#include <string_view>

namespace DeltaEngine
{
  class AssetKey
  {
  public:
    AssetKey() = default;
    explicit AssetKey(size_t digest) : _digest(digest) {}
    AssetKey(std::string_view str) : _digest(std::hash<std::string_view>{}(str)) {}
    bool operator==(const AssetKey &rhs) const { return _digest == rhs._digest; }
    size_t operator()() { return _digest; }

  private:
    size_t _digest{0};
  };
} // namespace DeltaEngine

namespace std
{
  template <>
  struct hash<DeltaEngine::AssetKey>
  {
    std::size_t operator()(DeltaEngine::AssetKey key) const
    {
      return key();
    }
  };
} // namespace std