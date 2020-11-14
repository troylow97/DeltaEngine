#include "Core/Debugging/Assert.h"

namespace DeltaEngine
{
  //*******************************************************************
  // Class Asset
  //*******************************************************************

  // Copy Constructor
  template <typename T1, typename T2>
  Asset<T1, T2>::Asset(const Asset& rhs) : m_group{rhs.m_group},
                                           m_data{rhs.m_data},
                                           m_key{rhs.m_key},
                                           m_timestamp{rhs.m_timestamp},
                                           m_state{rhs.m_state}
  {
    if (m_group)
      m_group->IncrementReferenceCount(m_key);
  }

  // Copy Assignment
  template <typename T1, typename T2>
  Asset<T1, T2>& Asset<T1, T2>::operator=(const Asset& rhs)
  {
    if (m_group)
      m_group->DecrementReferenceCount(m_key);

    m_group = rhs.m_group;
    m_data = rhs.m_data;
    m_key = rhs.m_key;
    m_timestamp = rhs.m_timestamp;
    m_state = rhs.m_state;

    if (m_group)
      m_group->IncrementReferenceCount(m_key);

    return *this;
  }

  // Move Constructor
  template <typename T1, typename T2>
  Asset<T1, T2>::Asset(Asset&& rhs) noexcept : m_group{rhs.m_group},
                                               m_data{rhs.m_data},
                                               m_key{rhs.m_key},
                                               m_timestamp{rhs.m_timestamp},
                                               m_state{rhs.m_state}
  {
    rhs.m_group = nullptr;
    rhs.m_data = nullptr;
    rhs.m_key = {};
    rhs.m_timestamp = 0;
    rhs.m_state = AssetState::Final;
  }

  // Move Assignment
  template <typename T1, typename T2>
  Asset<T1, T2>& Asset<T1, T2>::operator=(Asset&& rhs) noexcept
  {
    std::swap(m_group, rhs.m_group);
    std::swap(m_data, rhs.m_data);
    std::swap(m_key, rhs.m_key);
    std::swap(m_timestamp, rhs.m_timestamp);
    std::swap(m_state, rhs.m_state);
    return *this;
  }

  // Destructor
  template <typename T1, typename T2>
  Asset<T1, T2>::~Asset()
  {
    if (m_group)
      m_group->DecrementReferenceCount(m_key);
  }

  template <typename T1, typename T2>
  bool Asset<T1, T2>::operator==(const Asset& rhs) const
  {
    return m_group == rhs.m_group && m_key == rhs.m_key;
  }

  template <typename T1, typename T2>
  bool Asset<T1, T2>::operator!=(const Asset& rhs) const
  {
    return !operator==(rhs);
  }

  template <typename T1, typename T2>
  Asset<T1, T2>::operator bool()
  {
    Acquire();
    return m_data;
  }

  template <typename T1, typename T2>
  Asset<T1, T2>::operator T2*()
  {
    Acquire();
    return static_cast<T2*>(m_data);
  }

  template <typename T1, typename T2>
  T2& Asset<T1, T2>::operator*()
  {
    Acquire();
    ASSERT_ERROR(m_data, "Asset: accessing not loaded data with key")
    return *static_cast<T2*>(m_data);
  }

  template <typename T1, typename T2>
  T2* Asset<T1, T2>::operator->()
  {
    Acquire();
    ASSERT_ERROR(m_data, "Asset: accessing not loaded data with key")
    return static_cast<T2*>(m_data);
  }

  template <typename T1, typename T2>
  AssetKey Asset<T1, T2>::Key() const
  {
    return m_key;
  }

  template <typename T1, typename T2>
  AssetState Asset<T1, T2>::State()
  {
    Acquire();
    return m_state;
  }

  // Private Constructor
  template <typename T1, typename T2>
  Asset<T1, T2>::Asset(AssetGroup<T1>* group, AssetKey key) : m_group{group},
                                                              m_data{nullptr},
                                                              m_key{key},
                                                              m_timestamp{0},
                                                              m_state{AssetState::NotLoaded}
  {
    if (m_group)
      m_group->IncrementReferenceCount(m_key);
  }

  // Internal Acquire Asset
  template <typename T1, typename T2>
  void Asset<T1, T2>::Acquire()
  {
    if (m_state == AssetState::Final)
      return;

    if (m_group->Timestamp() <= m_timestamp)
      return;

    m_timestamp = m_group->Timestamp();

    const AssetData<T1>& d = m_group->m_datas[m_key];
    m_data = d.data;
    m_state = d.state;

    if (!m_data)
    {
      if (m_data = m_group->Fallback(), m_data)
      {
        if (m_state == AssetState::Loading)
          m_state = AssetState::LoadingFallback;
        else if (m_state == AssetState::NotFound)
          m_state = AssetState::NotFoundFallback;
        else
          m_state = AssetState::NotLoadedFallback;
      }
      else if (m_state != AssetState::Loading && m_state != AssetState::NotFound)
        m_state = AssetState::NotLoaded;
    }
  }
} // namespace DeltaEngine
