#pragma once
#include <string_view>

namespace DeltaEngine
{
class AssetKey
{
public:

  AssetKey() = default;

  explicit AssetKey( const size_t digest ) :
    m_digest( digest )
  {}

  AssetKey( const std::string str ) :
    m_digest( std::hash<std::string>{}( str ) )
  {}

  template <size_t Size>
  constexpr AssetKey( const char( &str )[Size] ) :
    m_digest( std::hash<std::string>{}( std::string( str ) ) )
  {}

  bool operator==( const AssetKey &rhs ) const
  {
    return m_digest == rhs.m_digest;
  }

  size_t operator()() const
  {
    return m_digest;
  }

private:
  size_t m_digest { 0 };
};
} // namespace DeltaEngine

namespace std
{
template <>
struct hash<DeltaEngine::AssetKey>
{
  std::size_t operator()( const DeltaEngine::AssetKey key ) const noexcept
  {
    return key();
  }
};
} // namespace std
