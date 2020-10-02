#pragma once

namespace DeltaEngine
{
  namespace Internal
  {
    template <typename T>
    void safe_delete(T* data)
    {
      assert(sizeof( T ) > 0);
      delete data;
    }
  } // namespace Internal

  template <typename T1>
  struct AssetData
  {
    T1* _data{nullptr};
    AssetState _state{AssetState::Mutable};
    AssetLifetime _lifetime{AssetLifetime::Managed};
    size_t _referenceCount{0};

    explicit AssetData() = default;

    // No Copy
    AssetData(const AssetData&) = delete;
    AssetData& operator=(const AssetData&) = delete;

    // Move Construction, no move assignment
    AssetData(AssetData&& rhs) noexcept : _data{rhs._data},
                                          _state{rhs._state},
                                          _lifetime{rhs._lifetime},
                                          _referenceCount{rhs._referenceCount}
    {
      rhs._data = nullptr;
      rhs._referenceCount = 0;
    }

    AssetData& operator=(AssetData&&) = delete;

    ~AssetData()
    {
      assert(_referenceCount == 0);
      Internal::safe_delete(_data);
    }
  };
} // namespace DeltaEngine
