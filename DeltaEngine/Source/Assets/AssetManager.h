#pragma once

#include "Asset.h"
#include "IAssetLoader.h"

namespace DeltaEngine
{

  template <typename...>
  struct AssetTypePack
  {
  };

  template <typename... Ts>
  class AssetManager : private AssetGroup<Ts>...
  {
  public:
    explicit AssetManager() = default;

    ~AssetManager();

    // Assets Group
    template <typename T1>
    size_t size() const;

    template <typename T1>
    size_t reference_count(AssetKey key) const;

    template <typename T1>
    AssetState state(AssetKey key) const;

    // Assets
    template <typename T1, typename T2 = T1>
    Asset<T1, T2> get(AssetKey key);

    template <typename T1>
    AssetManager<Ts...> &set(AssetKey key, T1 *data, AssetState state, AssetLifetime lifetime);

    template <typename T1>
    AssetManager<Ts...> &set(AssetKey key, T1 &&data, AssetState state, AssetLifetime lifetime);

    template <typename T1>
    AssetManager<Ts...> &set(AssetKey key, T1 *data);

    template <typename T1>
    AssetManager<Ts...> &set(AssetKey key, T1 &&data);

    template <typename T1>
    AssetManager<Ts...> &free();

    AssetManager<Ts...> &free();

    template <typename T1>
    AssetManager<Ts...> &clear();

    AssetManager<Ts...> &clear();

    // Fallbacks
    template <typename T1>
    T1 *fallback();

    template <typename T1>
    const T1 *fallback() const;

    template <typename T1>
    AssetManager<Ts...> &set_fallback(T1 *data);

    template <typename T1>
    AssetManager<Ts...> &set_fallback(T1 &&data);

    // Loaders
    template <typename T1>
    IAssetLoader<T1> *loader();

    template <typename T1>
    const IAssetLoader<T1> *loader() const;

    template <typename T1>
    AssetManager<Ts...> &set_loader(IAssetLoader<T1> *loader);

  private:
    template <typename T1, typename... T1s>
    void free_internal(AssetTypePack<T1, T1s...>);
    void free_internal(AssetTypePack<>) const;

    template <typename T1, typename... T1s>
    void clear_internal(AssetTypePack<T1, T1s...>);
    void clear_internal(AssetTypePack<>) const;

    template <typename T1, typename... T1s>
    void free_loaders(AssetTypePack<T1, T1s...>);
    void free_loaders(AssetTypePack<>) const;
  };

} // namespace DeltaEngine

#include "AssetManager.inl"