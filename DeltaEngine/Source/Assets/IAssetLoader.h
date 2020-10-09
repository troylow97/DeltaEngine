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
    void load( AssetKey key, std::string_view str );

  protected:
    void set(AssetKey key, T1 *data, AssetState state, AssetLifetime lifetime);

    template <typename T1s,
              typename = typename std::enable_if_t<
                  !std::is_same_v<typename std::decay_t<T1s>, std::nullptr_t>>>
    void set(AssetKey key, T1s &&data, AssetState state, AssetLifetime lifetime);

    void set(AssetKey key, T1 *data);

    template <typename T1s,
              typename = typename std::enable_if_t<
                  !std::is_same_v<typename std::decay_t<T1s>, std::nullptr_t>>>
    void set(AssetKey key, T1s &&data);

    void set_notFound(AssetKey key);
    
    // serialization
    virtual void doLoad(AssetKey key) = 0;
    virtual void doLoad( AssetKey key, std::string_view str ) = 0;
  };
} // namespace DeltaEngine

#include "IAssetLoader.inl"