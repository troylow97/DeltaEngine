#pragma once

#include "AssetGroup.h"

namespace DeltaEngine
{

  template <typename T1>
  class IAssetLoader
  {
    friend class AssetGroup<T1>;

    AssetGroup<T1> *_group{nullptr};
    size_t _requests{0};
    size_t _loaded{0};
    size_t _notFound{0};

  public:
    explicit IAssetLoader() = default;

    virtual ~IAssetLoader();

    size_t request_count() const;
    size_t loaded_count() const;
    size_t notFound_count() const;
    
    void load(AssetKey key);

  protected:
    void set(AssetKey key, T1 *data, AssetState state, AssetLifetime lifetime);

    template <typename T2,
              typename = typename std::enable_if_t<
                  !std::is_same_v<typename std::decay_t<T2>, std::nullptr_t>>>
    void set(AssetKey key, T2 &&data, AssetState state, AssetLifetime lifetime);

    void set(AssetKey key, T1 *data);

    template <typename T2,
              typename = typename std::enable_if_t<
                  !std::is_same_v<typename std::decay_t<T2>, std::nullptr_t>>>
    void set(AssetKey key, T2 &&data);

    void set_notFound(AssetKey key);
    
    virtual void doLoad(AssetKey key) = 0;
  };
} // namespace DeltaEngine

#include "IAssetLoader.inl"