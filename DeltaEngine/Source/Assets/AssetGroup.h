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

    void IncrementReferenceCount(AssetKey key);
    void DecrementReferenceCount(AssetKey key);

    std::unordered_map<AssetKey, AssetData<T1>> m_datas;
    T1* m_fallback{nullptr};
    AbstractLoader<T1>* m_loader{nullptr};
    size_t m_timestamp{0};

  protected:
    AssetGroup() = default;

  public:

    // Disable copy
    AssetGroup(const AssetGroup&) = delete;
    AssetGroup& operator=(const AssetGroup&) = delete;

    // Disable move
    AssetGroup(AssetGroup&&) = delete;
    AssetGroup& operator=(AssetGroup&&) = delete;

    // Destructor
    virtual ~AssetGroup();

    // Asset group
    [[nodiscard]] size_t Size() const;
    [[nodiscard]] size_t Timestamp() const;
    void Free();
    void Clear();

    // Asset data
    void Load(AssetKey key);
    void Load(AssetKey key, std::string_view str);

    template <typename T2>
    Asset<T1, T2> Get(AssetKey key);

    void Set(AssetKey key, T1* data, AssetState state, AssetLifetime lifetime);
    [[nodiscard]] size_t ReferenceCount(AssetKey key) const;
    [[nodiscard]] AssetState State(AssetKey key) const;

    // Fallback
    T1* Fallback();
    [[nodiscard]] const T1* Fallback() const;
    void SetFallback(T1* data);

    // Loaders
    AbstractLoader<T1>* Loader();
    [[nodiscard]] const AbstractLoader<T1>* Loader() const;
    void SetLoader(AbstractLoader<T1>* loader);
    void FreeLoader();
  };
} // namespace DeltaEngine

#include "AssetGroup.inl"
