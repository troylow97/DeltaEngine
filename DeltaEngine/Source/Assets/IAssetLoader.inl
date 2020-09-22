namespace DeltaEngine
{

  template <typename T1>
  IAssetLoader<T1>::~IAssetLoader()
  {
    if (_group)
      _group->_loader = nullptr;
  }

  template <typename T1>
  size_t IAssetLoader<T1>::request_count() const
  {
    return _requests;
  }

  template <typename T1>
  size_t IAssetLoader<T1>::loaded_count() const
  {
    return _loaded;
  }

  template <typename T1>
  size_t IAssetLoader<T1>::notFound_count() const
  {
    return _notFound;
  }

  template <typename T1>
  void IAssetLoader<T1>::load(AssetKey key)
  {
    _requests++;
    _group->set(key, nullptr, AssetState::Loading, AssetLifetime::Persistant);

    doLoad(key);
  }

  template <typename T1>
  void IAssetLoader<T1>::set(AssetKey key, T1 *data, AssetState state, AssetLifetime lifetime)
  {
    if (data)
      _loaded++;

    if (!data && state == AssetState::NotFound)
      _notFound++;

    _group->set(key, data, state, lifetime);
  }

  template <typename T1>
  template <typename T2, typename>
  void IAssetLoader<T1>::set(AssetKey key, T2 &&data, AssetState state, AssetLifetime lifetime)
  {
    set(key, new std::decay_t<T2>(std::forward<T2>(data)), state, lifetime);
  }

  template <typename T1>
  void IAssetLoader<T1>::set(AssetKey key, T1 *data)
  {
    set(key, data, AssetState::Final, AssetLifetime::Persistant);
  }

  template <typename T1>
  template <typename T2, typename>
  void IAssetLoader<T1>::set(AssetKey key, T2 &&data)
  {
    set(key, new std::decay_t<T2>(std::forward<T2>(data)));
  }

  template <typename T1>
  void IAssetLoader<T1>::set_notFound(AssetKey key)
  {
    set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistant);
  }

} // namespace DeltaEngine