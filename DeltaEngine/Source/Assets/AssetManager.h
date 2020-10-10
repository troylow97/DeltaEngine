#pragma once

#include "Asset.h"
#include "AbstractLoader.h"

namespace DeltaEngine
{
template <typename...>
struct AssetTypePack
{};

template <typename... Ts>
class AssetManager final : private AssetGroup<Ts>...
{
public:
  explicit AssetManager() = default;

  ~AssetManager();

  // Assets Group
  template <typename T1>
  [[nodiscard]] size_t Size() const;

  template <typename T1>
  [[nodiscard]] size_t ReferenceCount( AssetKey key ) const;

  template <typename T1>
  [[nodiscard]] AssetState State( AssetKey key ) const;

  // Assets
  template <typename T1>
  AssetManager<Ts...> &Load( AssetKey key );

  template <typename T1>
  AssetManager<Ts...> &Load( AssetKey key, std::string_view str );

  template <typename T1, typename T2 = T1>
  Asset<T1, T2> Get( AssetKey key );

  template <typename T1>
  AssetManager<Ts...> &Set( AssetKey key, T1 *data, AssetState state, AssetLifetime lifetime );

  template <typename T1>
  AssetManager<Ts...> &Set( AssetKey key, T1 &&data, AssetState state, AssetLifetime lifetime );

  template <typename T1>
  AssetManager<Ts...> &Set( AssetKey key, T1 *data );

  template <typename T1>
  AssetManager<Ts...> &Set( AssetKey key, T1 &&data );

  // Data whose lifetime is managed
  template <typename T1>
  AssetManager<Ts...> &Free();

  AssetManager<Ts...> &Free();

  template <typename T1>
  AssetManager<Ts...> &Clear();

  AssetManager<Ts...> &Clear();

  // Fallbacks
  template <typename T1>
  T1 *Fallback();

  template <typename T1>
  const T1 *Fallback() const;

  template <typename T1>
  AssetManager<Ts...> &SetFallback( T1 *data );

  template <typename T1>
  AssetManager<Ts...> &SetFallback( T1 &&data );

  // Loaders
  template <typename T1>
  AbstractLoader<T1> *Loader();

  template <typename T1>
  const AbstractLoader<T1> *Loader() const;

  template <typename T1>
  AssetManager<Ts...> &SetLoader( AbstractLoader<T1> *loader );

private:
  template <typename T1, typename... T1s>
  void FreeInternal( AssetTypePack<T1, T1s...> );
  void FreeInternal( AssetTypePack<> ) const;

  template <typename T1, typename... T1s>
  void ClearInternal( AssetTypePack<T1, T1s...> );
  void ClearInternal( AssetTypePack<> ) const;

  template <typename T1, typename... T1s>
  void FreeLoaders( AssetTypePack<T1, T1s...> );
  void FreeLoaders( AssetTypePack<> ) const;
};
} // namespace DeltaEngine

#include "AssetManager.inl"
