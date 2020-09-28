namespace DeltaEngine
{

  template <typename... Ts>
  AssetManager<Ts...>::~AssetManager()
  {
    free_loaders(AssetTypePack<Ts...>{});
  }

  template <typename... Ts>
  template <typename T1>
  size_t AssetManager<Ts...>::size() const
  {
    return AssetGroup<T1>::size();
  }

  template <typename... Ts>
  template <typename T1>
  size_t AssetManager<Ts...>::reference_count(AssetKey key) const
  {
    return AssetGroup<T1>::reference_count(key);
  }

  template <typename... Ts>
  template <typename T1>
  AssetState AssetManager<Ts...>::state(AssetKey key) const
  {
    return AssetGroup<T1>::state(key);
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::load( AssetKey key )
  {
    AssetGroup<T1>::load( key );
    return *this;
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::load( AssetKey key, std::string_view str )
  {
    AssetGroup<T1>::load( key, str );
    return *this;
  }

  template <typename... Ts>
  template <typename T1, typename T2>
  Asset<T1, T2> AssetManager<Ts...>::get(AssetKey key)
  {
    return AssetGroup<T1>::template get<T2>(key);
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::set(AssetKey key, T1 *data, AssetState state, AssetLifetime lifetime)
  {
    AssetGroup<T1>::set(key, data, state, lifetime);
    return *this;
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::set(AssetKey key, T1 &&data, AssetState state, AssetLifetime lifetime)
  {
    return set(key, new std::decay_t<T1>(std::forward<T1>(data)), state, lifetime);
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::set(AssetKey key, T1 *data)
  {
    return set(key, data, AssetState::Final, AssetLifetime::Persistant);
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::set(AssetKey key, T1 &&data)
  {
    return set(key, new std::decay_t<T1>(std::forward<T1>(data)));
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::free()
  {
    AssetGroup<T1>::free();
    return *this;
  }

  template <typename... Ts>
  AssetManager<Ts...> &AssetManager<Ts...>::free()
  {
    free_internal(AssetTypePack<Ts...>{});
    return *this;
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::clear()
  {
    AssetGroup<T1>::clear();
    return *this;
  }

  template <typename... Ts>
  AssetManager<Ts...> &AssetManager<Ts...>::clear()
  {
    clear_internal(AssetTypePack<Ts...>{});
    return *this;
  }

  template <typename... Ts>
  template <typename T1>
  T1 *AssetManager<Ts...>::fallback()
  {
    return AssetGroup<T1>::fallback();
  }

  template <typename... Ts>
  template <typename T1>
  const T1 *AssetManager<Ts...>::fallback() const
  {
    return AssetGroup<T1>::fallback();
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::set_fallback(T1 *data)
  {
    AssetGroup<T1>::set_fallback(data);
    return *this;
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::set_fallback(T1 &&data)
  {
    return set_fallback(new std::decay_t<T1>(std::forward<T1>(data)));
  }

  template <typename... Ts>
  template <typename T1>
  AbstractLoader<T1> *AssetManager<Ts...>::loader()
  {
    return AssetGroup<T1>::loader();
  }

  template <typename... Ts>
  template <typename T1>
  const AbstractLoader<T1> *AssetManager<Ts...>::loader() const
  {
    return AssetGroup<T1>::loader();
  }

  template <typename... Ts>
  template <typename T1>
  AssetManager<Ts...> &AssetManager<Ts...>::set_loader(AbstractLoader<T1> *loader)
  {
    AssetGroup<T1>::set_loader(loader);
    return *this;
  }

  template <typename... Ts>
  template <typename T1, typename... T1s>
  void AssetManager<Ts...>::free_internal(AssetTypePack<T1, T1s...>)
  {
    free<T1>();
    free_internal(AssetTypePack<T1s...>{});
  }

  template <typename... Ts>
  void AssetManager<Ts...>::free_internal(AssetTypePack<>) const
  {
  }

  template <typename... Ts>
  template <typename T1, typename... T1s>
  void AssetManager<Ts...>::clear_internal(AssetTypePack<T1, T1s...>)
  {
    clear<T1>();
    clear_internal(AssetTypePack<T1s...>{});
  }

  template <typename... Ts>
  void AssetManager<Ts...>::clear_internal(AssetTypePack<>) const
  {
  }

  template <typename... Ts>
  template <typename T1, typename... T1s>
  void AssetManager<Ts...>::free_loaders(AssetTypePack<T1, T1s...>)
  {
    AssetGroup<T1>::free_loader();
    free_loaders(AssetTypePack<T1s...>{});
  }

  template <typename... Ts>
  void AssetManager<Ts...>::free_loaders(AssetTypePack<>) const
  {
  }

} // namespace DeltaEngine