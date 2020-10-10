#include "Core/Debugging/Assert.h"
namespace DeltaEngine
{
template <typename T1>
AssetGroup<T1>::~AssetGroup()
{
  if ( m_fallback )
    Internal::safe_delete( m_fallback );
}

//****************************************************************************
// Asset Group
//****************************************************************************

template <typename T1>
size_t AssetGroup<T1>::Size() const
{
  return m_datas.size();
}

template <typename T1>
size_t AssetGroup<T1>::Timestamp() const
{
  return m_timestamp;
}

template <typename T1>
void AssetGroup<T1>::Free()
{
  for ( auto it = m_datas.begin(); it != m_datas.end();)
    if ( it->second._lifetime == AssetLifetime::Managed )
      it = m_datas.erase( it );
    else
      ++it;
}

template <typename T1>
void AssetGroup<T1>::Clear()
{
  m_datas.clear();
}

//****************************************************************************
// Asset Data
//****************************************************************************

template <typename T1>
void AssetGroup<T1>::Load( AssetKey key )
{
  if ( m_loader )
    m_loader->Load( key );
}

template <typename T1>
void AssetGroup<T1>::Load( AssetKey key, std::string_view str )
{
  if ( m_loader )
    m_loader->Load( key, str );
}

template <typename T1>
template <typename T2>
Asset<T1, T2> AssetGroup<T1>::Get( AssetKey key )
{
  if ( auto it = m_datas.find( key ); it == m_datas.end() )
  {
    Set( key, nullptr, AssetState::NotFound, AssetLifetime::Managed );
    Load( key );
  }

  return Asset( this, key );
}

template <typename T1>
void AssetGroup<T1>::Set( const AssetKey key, T1 *const data, const AssetState state, const AssetLifetime lifetime )
{
  auto it = m_datas.find( key );

  ASSERT_ERROR( ( data == nullptr ) == ( state == AssetState::NotFound || state == AssetState::Loading ),
                "AssetGroup: data should be null only if state is NotFound or Loading" );

  ASSERT_ERROR( ( it == m_datas.end() || it->second.state != AssetState::Final ),
                "AssertGroup: cannot change data set as final" );

  if ( it == m_datas.end() )
    it = m_datas.emplace( key, AssetData<T1>() ).first;
  else
    Internal::safe_delete( it->second.data );

  it->second.data = data;
  it->second.state = state;
  it->second.lifetime = lifetime;
  ++m_timestamp;
}

template <typename T1>
size_t AssetGroup<T1>::ReferenceCount( const AssetKey key ) const
{
  const auto it = m_datas.find( key );
  if ( it == m_datas.end() )
    return 0;
  return it->second._referenceCount;
}

template <typename T1>
AssetState AssetGroup<T1>::State( const AssetKey key ) const
{
  const auto it = m_datas.find( key );

  if ( it == m_datas.end() )
  {
    if ( m_fallback )
      return AssetState::NotLoadedFallback;
    return AssetState::NotLoaded;
  }

  if ( !it->second._data )
  {
    if ( m_fallback )
    {
      if ( it->second._state == AssetState::Loading )
        return AssetState::LoadingFallback;
      if ( it->second._state == AssetState::NotFound )
        return AssetState::NotFoundFallback;
      return AssetState::NotLoadedFallback;
    }
    return AssetState::NotLoaded;
  }

  return it->second._state;
}

//****************************************************************************
// Fallback
//****************************************************************************
template <typename T1>
T1 *AssetGroup<T1>::Fallback()
{
  return m_fallback;
}

template <typename T1>
const T1 *AssetGroup<T1>::Fallback() const
{
  return m_fallback;
}

template <typename T1>
void AssetGroup<T1>::SetFallback( T1 *const data )
{
  Internal::safe_delete( m_fallback );
  m_fallback = data;
  ++m_timestamp;
}

//****************************************************************************
// Loaders
//****************************************************************************

template <typename T1>
AbstractLoader<T1> *AssetGroup<T1>::Loader()
{
  return m_loader;
}

template <typename T1>
const AbstractLoader<T1> *AssetGroup<T1>::Loader() const
{
  return m_loader;
}

template <typename T1>
void AssetGroup<T1>::SetLoader( AbstractLoader<T1> *const loader )
{
  delete m_loader;

  if ( m_loader = loader; m_loader )
    m_loader->m_group = this;
}

template <typename T1>
void AssetGroup<T1>::FreeLoader()
{
  if ( !m_loader )
    return;

  delete m_loader;
}

//****************************************************************************
// Reference Counting
//****************************************************************************

template <typename T1>
void AssetGroup<T1>::IncrementReferenceCount( AssetKey key )
{
  auto it = m_datas.find( key );
  if ( it != m_datas.end() )
    ++it->second.reference_count;
}

template <typename T1>
void AssetGroup<T1>::DecrementReferenceCount( AssetKey key )
{
  auto it = m_datas.find( key );
  ASSERT_ERROR( it != m_datas.end(),
                "AssetGroup: Decrementing reference with key {} that does not exist" )
  if ( --( it->second.reference_count ) == 0 &&
       it->second.lifetime == AssetLifetime::ReferenceCounted )
    m_datas.erase( it );
}

} // namespace DeltaEngine
