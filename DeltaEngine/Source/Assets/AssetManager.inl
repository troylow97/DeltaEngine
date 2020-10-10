namespace DeltaEngine
{
template <typename... Ts>
AssetManager<Ts...>::~AssetManager()
{
  FreeLoaders( AssetTypePack<Ts...>{} );
}

template <typename... Ts>
template <typename T1>
size_t AssetManager<Ts...>::Size() const
{
  return AssetGroup<T1>::Size();
}

template <typename... Ts>
template <typename T1>
size_t AssetManager<Ts...>::ReferenceCount( AssetKey key ) const
{
  return AssetGroup<T1>::ReferenceCount( key );
}

template <typename... Ts>
template <typename T1>
AssetState AssetManager<Ts...>::State( AssetKey key ) const
{
  return AssetGroup<T1>::State( key );
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::Load( AssetKey key )
{
  AssetGroup<T1>::Load( key );
  return *this;
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::Load( AssetKey key, std::string_view str )
{
  AssetGroup<T1>::Load( key, str );
  return *this;
}

template <typename... Ts>
template <typename T1, typename T2>
Asset<T1, T2> AssetManager<Ts...>::Get( AssetKey key )
{
  return AssetGroup<T1>::template Get<T2>( key );
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::Set( AssetKey key, T1 *data, AssetState state, AssetLifetime lifetime )
{
  AssetGroup<T1>::Set( key, data, state, lifetime );
  return *this;
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::Set( AssetKey key, T1 &&data, AssetState state, AssetLifetime lifetime )
{
  return Set( key, new std::decay_t<T1>( std::forward<T1>( data ) ), state, lifetime );
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::Set( AssetKey key, T1 *data )
{
  return Set( key, data, AssetState::Final, AssetLifetime::Persistent );
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::Set( AssetKey key, T1 &&data )
{
  return Set( key, new std::decay_t<T1>( std::forward<T1>( data ) ) );
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::Free()
{
  AssetGroup<T1>::Free();
  return *this;
}

template <typename... Ts>
AssetManager<Ts...> &AssetManager<Ts...>::Free()
{
  FreeInternal( AssetTypePack<Ts...>{} );
  return *this;
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::Clear()
{
  AssetGroup<T1>::Clear();
  return *this;
}

template <typename... Ts>
AssetManager<Ts...> &AssetManager<Ts...>::Clear()
{
  ClearInternal( AssetTypePack<Ts...>{} );
  return *this;
}

template <typename... Ts>
template <typename T1>
T1 *AssetManager<Ts...>::Fallback()
{
  return AssetGroup<T1>::Fallback();
}

template <typename... Ts>
template <typename T1>
const T1 *AssetManager<Ts...>::Fallback() const
{
  return AssetGroup<T1>::Fallback();
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::SetFallback( T1 *data )
{
  AssetGroup<T1>::SetFallback( data );
  return *this;
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::SetFallback( T1 &&data )
{
  return SetFallback( new std::decay_t<T1>( std::forward<T1>( data ) ) );
}

template <typename... Ts>
template <typename T1>
AbstractLoader<T1> *AssetManager<Ts...>::Loader()
{
  return AssetGroup<T1>::Loader();
}

template <typename... Ts>
template <typename T1>
const AbstractLoader<T1> *AssetManager<Ts...>::Loader() const
{
  return AssetGroup<T1>::Loader();
}

template <typename... Ts>
template <typename T1>
AssetManager<Ts...> &AssetManager<Ts...>::SetLoader( AbstractLoader<T1> *loader )
{
  AssetGroup<T1>::SetLoader( loader );
  return *this;
}

template <typename... Ts>
template <typename T1, typename... T1s>
void AssetManager<Ts...>::FreeInternal( AssetTypePack<T1, T1s...> )
{
  Free<T1>();
  FreeInternal( AssetTypePack<T1s...>{} );
}

template <typename... Ts>
void AssetManager<Ts...>::FreeInternal( AssetTypePack<> ) const
{}

template <typename... Ts>
template <typename T1, typename... T1s>
void AssetManager<Ts...>::ClearInternal( AssetTypePack<T1, T1s...> )
{
  Clear<T1>();
  ClearInternal( AssetTypePack<T1s...>{} );
}

template <typename... Ts>
void AssetManager<Ts...>::ClearInternal( AssetTypePack<> ) const
{}

template <typename... Ts>
template <typename T1, typename... T1s>
void AssetManager<Ts...>::FreeLoaders( AssetTypePack<T1, T1s...> )
{
  AssetGroup<T1>::FreeLoader();
  FreeLoaders( AssetTypePack<T1s...>{} );
}

template <typename... Ts>
void AssetManager<Ts...>::FreeLoaders( AssetTypePack<> ) const
{}
} // namespace DeltaEngine
