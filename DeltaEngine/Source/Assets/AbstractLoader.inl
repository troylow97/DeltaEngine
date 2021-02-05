/**********************************************************************************
* \file   AbstractLoader.inl
* \brief  This file contains the implementation of AbstractLoader
*         AbstractLoader is an interface class that will be specialized for each
*         resource type T
*
*         AbstractLoader support the following functionality:
*         - Overloaded load
*         - Request count tracking
*         - Loaded count tracking
*         - Not found count tracking
*
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

namespace DeltaEngine
{
  template <typename T1>
  AbstractLoader<T1>::~AbstractLoader()
  {
    if (m_group)
      m_group->m_loader = nullptr;
  }

  template <typename T1>
  size_t AbstractLoader<T1>::RequestCount() const
  {
    return m_requests;
  }

  template <typename T1>
  size_t AbstractLoader<T1>::LoadedCount() const
  {
    return m_loaded;
  }

  template <typename T1>
  size_t AbstractLoader<T1>::NotFoundCount() const
  {
    return m_not_found;
  }

  template <typename T1>
  void AbstractLoader<T1>::Load()
  {
    DoLoad();
  }


  template <typename T1>
  void AbstractLoader<T1>::Load(AssetKey key)
  {
    m_requests++;
    DoLoad(key);
  }

  template <typename T1>
  void AbstractLoader<T1>::Load(AssetKey key, std::string_view str)
  {
    m_requests++;
    DoLoad(key, str);
  }

  template <typename T1>
  void AbstractLoader<T1>::Set(AssetKey key, T1* data, AssetState state, AssetLifetime lifetime)
  {
    if (data)
      m_loaded++;

    if (!data && state == AssetState::NotFound)
      m_not_found++;

    m_group->Set(key, data, state, lifetime);
  }

  template <typename T1>
  template <typename T1s, typename>
  void AbstractLoader<T1>::Set(AssetKey key, T1s&& data, AssetState state, AssetLifetime lifetime)
  {
    Set(key, new std::decay_t<T1s>(std::forward<T1s>(data)), state, lifetime);
  }

  template <typename T1>
  void AbstractLoader<T1>::Set(AssetKey key, T1* data)
  {
    Set(key, data, AssetState::Final, AssetLifetime::Persistent);
  }

  template <typename T1>
  template <typename T1s, typename>
  void AbstractLoader<T1>::Set(AssetKey key, T1s&& data)
  {
    Set(key, new std::decay_t<T1s>(std::forward<T1s>(data)));
  }

  template <typename T1>
  void AbstractLoader<T1>::SetNotFound(AssetKey key)
  {
    Set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
  }
} // namespace DeltaEngine
