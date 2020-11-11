#pragma once

#include "AssetGroup.h"

namespace DeltaEngine
{
template <typename T1>
class AbstractLoader
{
  friend class AssetGroup<T1>;

  AssetGroup<T1> *m_group { nullptr };
  size_t m_requests { 0 };
  size_t m_loaded { 0 };
  size_t m_not_found { 0 };

public:
  explicit AbstractLoader() = default;

  virtual ~AbstractLoader();

  [[nodiscard]] size_t RequestCount() const;
  [[nodiscard]] size_t LoadedCount() const;
  [[nodiscard]] size_t NotFoundCount() const;

  void Load();
  void Load( AssetKey key );
  void Load( AssetKey key, std::string_view str );

protected:
  void Set( AssetKey key, T1 *data, AssetState state, AssetLifetime lifetime );

  template <typename T1s,
    typename = std::enable_if_t<
    !std::is_same_v<std::decay_t<T1s>, std::nullptr_t>>>
    void Set( AssetKey key, T1s &&data, AssetState state, AssetLifetime lifetime );

  void Set( AssetKey key, T1 *data );

  template <typename T1s,
    typename = std::enable_if_t<
    !std::is_same_v<std::decay_t<T1s>, std::nullptr_t>>>
    void Set( AssetKey key, T1s &&data );

  void SetNotFound( AssetKey key );

  virtual void DoLoad() = 0;
  virtual void DoLoad( AssetKey key ) = 0;
  virtual void DoLoad( AssetKey key, std::string_view str ) = 0;
};
} // namespace DeltaEngine

#include "AbstractLoader.inl"
