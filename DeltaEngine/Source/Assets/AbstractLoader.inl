namespace DeltaEngine
{
  template <typename T1>
  AbstractLoader<T1>::~AbstractLoader()
  {
    if (_group)
      _group->_loader = nullptr;
  }

  template <typename T1>
  size_t AbstractLoader<T1>::request_count() const
  {
    return _requests;
  }

  template <typename T1>
  size_t AbstractLoader<T1>::loaded_count() const
  {
    return _loaded;
  }

  template <typename T1>
  size_t AbstractLoader<T1>::notFound_count() const
  {
    return _notFound;
  }

  template <typename T1>
  void AbstractLoader<T1>::load(AssetKey key)
  {
    _requests++;
    doLoad(key);
  }

  template <typename T1>
  void AbstractLoader<T1>::load(AssetKey key, std::string_view str)
  {
    _requests++;
    doLoad(key, str);
  }

  template <typename T1>
  void AbstractLoader<T1>::set(AssetKey key, T1* data, AssetState state, AssetLifetime lifetime)
  {
    if (data)
      _loaded++;

    if (!data && state == AssetState::NotFound)
      _notFound++;

    _group->set(key, data, state, lifetime);
  }

  template <typename T1>
  template <typename T1s, typename>
  void AbstractLoader<T1>::set(AssetKey key, T1s&& data, AssetState state, AssetLifetime lifetime)
  {
    set(key, new std::decay_t<T1s>(std::forward<T1s>(data)), state, lifetime);
  }

  template <typename T1>
  void AbstractLoader<T1>::set(AssetKey key, T1* data)
  {
    set(key, data, AssetState::Final, AssetLifetime::Persistent);
  }

  template <typename T1>
  template <typename T1s, typename>
  void AbstractLoader<T1>::set(AssetKey key, T1s&& data)
  {
    set(key, new std::decay_t<T1s>(std::forward<T1s>(data)));
  }

  template <typename T1>
  void AbstractLoader<T1>::set_not_found(AssetKey key)
  {
    set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
  }
} // namespace DeltaEngine
