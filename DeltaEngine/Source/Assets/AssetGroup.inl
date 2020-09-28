namespace DeltaEngine
{
  template <typename T1>
  AssetGroup<T1>::~AssetGroup()
  {
    Internal::safe_delete(_fallback);
  }

  //****************************************************************************
  // Asset Group
  //****************************************************************************

  template <typename T1>
  size_t AssetGroup<T1>::size() const
  {
    return _datas.size();
  }

  template <typename T1>
  size_t AssetGroup<T1>::timestamp() const
  {
    return _timestamp;
  }

  template <typename T1>
  void AssetGroup<T1>::free()
  {
    for (auto it = _datas.begin(); it != _datas.end();)
    {
      if (it->second._lifetime == AssetLifetime::Managed)
        it = _datas.erase(it);
      else
        it++;
    }
  }

  template <typename T1>
  void AssetGroup<T1>::clear()
  {
    _datas.clear();
  }

  //****************************************************************************
  // Asset Data
  //****************************************************************************

  template <typename T1>
  void AssetGroup<T1>::load( AssetKey key )
  {
    if ( _loader )
      _loader->load( key );
  }

  template <typename T1>
  void AssetGroup<T1>::load( AssetKey key, std::string_view str )
  {
    if ( _loader )
      _loader->load( key, str );
  }

  template <typename T1>
  template <typename T2>
  Asset<T1, T2> AssetGroup<T1>::get( AssetKey key)
  {
    if ( auto it = _datas.find( key ); it == _datas.end() )
    {
      set(key, nullptr, AssetState::NotFound, AssetLifetime::Managed);
      load( key );
    }

    return Asset(this, key);
  }

  template <typename T1>
  void AssetGroup<T1>::set(const AssetKey key, T1 *const data, const AssetState state, const AssetLifetime lifetime)
  {
    auto it = _datas.find(key);

    assert((data == nullptr) == (state == AssetState::NotFound || state == AssetState::Loading));

    assert(it == _datas.end() || it->second._state != AssetState::Final);

    if (it == _datas.end())
      it = _datas.emplace(key, AssetData<T1>()).first;
    else
      Internal::safe_delete(it->second._data);

    it->second._data = data;
    it->second._state = state;
    it->second._lifetime = lifetime;
    ++_timestamp;
  }

  template <typename T1>
  size_t AssetGroup<T1>::reference_count(const AssetKey key) const
  {
    const auto it = _datas.find(key);
    if (it == _datas.end())
      return 0;
    return it->second._referenceCount;
  }

  template <typename T1>
  AssetState AssetGroup<T1>::state(const AssetKey key) const
  {
    const auto it = _datas.find(key);

    if (it == _datas.end())
    {
      if (_fallback)
        return AssetState::NotLoadedFallback;
      else
        return AssetState::NotLoaded;
    }

    if (!it->second._data)
    {
      if (_fallback)
      {
        if (it->second._state == AssetState::Loading)
          return AssetState::LoadingFallback;
        else if (it->second._state == AssetState::NotFound)
          return AssetState::NotFoundFallback;
        else
          return AssetState::NotLoadedFallback;
      }
      else
        return AssetState::NotLoaded;
    }

    return it->second._state;
  }

  //****************************************************************************
  // Fallback
  //****************************************************************************
  template <typename T1>
  T1 *AssetGroup<T1>::fallback()
  {
    return _fallback;
  }

  template <typename T1>
  const T1 *AssetGroup<T1>::fallback() const
  {
    return _fallback;
  }

  template <typename T1>
  void AssetGroup<T1>::set_fallback(T1 *const data)
  {
    Internal::safe_delete(_fallback);
    _fallback = data;
    ++_timestamp;
  }

  //****************************************************************************
  // Loaders
  //****************************************************************************

  template <typename T1>
  AbstractLoader<T1> *AssetGroup<T1>::loader()
  {
    return _loader;
  }

  template <typename T1>
  const AbstractLoader<T1> *AssetGroup<T1>::loader() const
  {
    return _loader;
  }

  template <typename T1>
  void AssetGroup<T1>::set_loader(AbstractLoader<T1> *const loader)
  {
    delete _loader;

    if (_loader = loader; _loader)
      _loader->_group = this;
  }

  template <typename T1>
  void AssetGroup<T1>::free_loader()
  {
    if (!_loader)
      return;

    delete _loader;
  }

  //****************************************************************************
  // Reference Counting
  //****************************************************************************

  template <typename T1>
  void AssetGroup<T1>::increment_reference_count(AssetKey key)
  {
    auto it = _datas.find(key);
    if (it != _datas.end())
      it->second._referenceCount++;
  }

  template <typename T1>
  void AssetGroup<T1>::decrement_reference_count(AssetKey key)
  {
    auto it = _datas.find(key);
    if (it != _datas.end())
      if (--(it->second._referenceCount) == 0 &&
          it->second._lifetime == AssetLifetime::ReferenceCounted)
        _datas.erase(it);
  }

} // namespace DeltaEngine