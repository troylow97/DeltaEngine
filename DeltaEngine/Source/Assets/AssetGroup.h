#pragma once
#include "AssetData.h"
#include <cassert>
#include <unordered_map> 

namespace DeltaEngine
{

  template <typename>
  class AbstractLoader;

  class AssetKey;

  template <typename, typename>
  class Asset;

  template <typename T1>
  class AssetGroup
  {
    template <typename, typename>
    friend class Asset;

    friend class AbstractLoader<T1>;

    void increment_reference_count(AssetKey key);
    void decrement_reference_count(AssetKey key);

    std::unordered_map<AssetKey, AssetData<T1>> _datas;
    T1 *_fallback {nullptr};
    AbstractLoader<T1> *_loader{nullptr};
    size_t _timestamp{0};

  protected:
    AssetGroup() = default;

  public:
  
    // Disable copy
    AssetGroup(const AssetGroup &) = delete;
    AssetGroup &operator=(const AssetGroup &) = delete;

    // Disable move
    AssetGroup(AssetGroup &&) = delete;
    AssetGroup &operator=(AssetGroup &&) = delete;

    // Destructor
    virtual ~AssetGroup();

    // Asset group
    size_t size() const;
    size_t timestamp() const;
    void free();
    void clear();

    // Asset data
    void load( AssetKey key );
    void load( AssetKey key, std::string_view str );

    template <typename T2>
    Asset<T1, T2> get(AssetKey key);
    
    void set(AssetKey key, T1 *data, AssetState state, AssetLifetime lifetime);
    size_t reference_count(AssetKey key) const;
    AssetState state(AssetKey key) const;

    // Fallback
    T1 *fallback();
    const T1 *fallback() const;
    void set_fallback(T1 *data);

    // Loaders
    AbstractLoader<T1> *loader();
    const AbstractLoader<T1> *loader() const;
    void set_loader(AbstractLoader<T1> *loader);
    void free_loader();

  };

} // namespace DeltaEngine

#include "AssetGroup.inl"