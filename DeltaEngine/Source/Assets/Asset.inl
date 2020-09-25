namespace DeltaEngine
{
  //*******************************************************************
  // Class Asset
  //*******************************************************************

  // Copy Constructor
  template <typename T1, typename T2>
  Asset<T1, T2>::Asset(const Asset &rhs) : _group{rhs._group},
                                                    _data{rhs._data},
                                                    _key{rhs._key},
                                                    _timestamp{rhs._timestamp},
                                                    _state{rhs._state}
  {
    if (_group)
      _group->increment_reference_count(_key);
  }

  // Copy Assignment
  template <typename T1, typename T2>
  Asset<T1, T2> &Asset<T1, T2>::operator=(const Asset &rhs)
  {
    if (_group)
      _group->decrement_reference_count(_key);

    _group = rhs._group;
    _data = rhs._data;
    _key = rhs._key;
    _timestamp = rhs._timestamp;
    _state = rhs._state;

    if (_group)
      _group->increment_reference_count(_key);

    return *this;
  }

  // Move Constructor
  template <typename T1, typename T2>
  Asset<T1, T2>::Asset(Asset &&rhs) : _group{rhs._group},
                                      _data{rhs._data},
                                      _key{rhs._key},
                                      _timestamp{rhs._timestamp},
                                      _state{rhs._state}
  {
    rhs._group = nullptr;
    rhs._data = nullptr;
    rhs._key = {};
    rhs._timestamp = 0;
    rhs._state = AssetState::Final;
  }

  // Move Assignment
  template <typename T1, typename T2>
  Asset<T1, T2> &Asset<T1, T2>::operator=(Asset &&rhs)
  {
    std::swap(_group, rhs._group);
    std::swap(_data, rhs._data);
    std::swap(_key, rhs._key);
    std::swap(_timestamp, rhs._timestamp);
    std::swap(_state, rhs._state);
    return *this;
  }

  // Destructor
  template <typename T1, typename T2>
  Asset<T1, T2>::~Asset()
  {
    if (_group)
      _group->decrement_reference_count(_key);
  }

  template <typename T1, typename T2>
  bool Asset<T1, T2>::operator==(const Asset &rhs) const
  {
    return _group == rhs._group && _key == rhs._key;
  }

  template <typename T1, typename T2>
  bool Asset<T1, T2>::operator!=(const Asset &rhs) const
  {
    return !operator==(rhs);
  }
  template <typename T1, typename T2>
  Asset<T1, T2>::operator bool()
  {
    acquire();
    return _data;
  }

  template <typename T1, typename T2>
  Asset<T1, T2>::operator T2 *()
  {
    acquire();
    return static_cast<T2 *>(_data);
  }

  template <typename T1, typename T2>
  T2 &Asset<T1, T2>::operator*()
  {
    acquire();
    return *static_cast<T2 *>(_data);
  }

  template <typename T1, typename T2>
  T2 *Asset<T1, T2>::operator->()
  {
    acquire();
    return static_cast<T2 *>(_data);
  }

  template <typename T1, typename T2>
  AssetKey Asset<T1, T2>::key()
  {
    return _key;
  }

  template <typename T1, typename T2>
  AssetState Asset<T1, T2>::state()
  {
    acquire();
    return _state;
  }

  // Private Constructor
  template <typename T1, typename T2>
  Asset<T1, T2>::Asset(AssetGroup<T1> *group, AssetKey key) : _group{group},
                                                              _data{nullptr},
                                                              _key{key},
                                                              _timestamp{0},
                                                              _state{AssetState::NotLoaded}
  {
    if (_group)
      _group->increment_reference_count(_key);
  }

  // Internal Acquire Asset
  template <typename T1, typename T2>
  void Asset<T1, T2>::acquire()
  {
    if (_state == AssetState::Final)
      return;

    if (_group->timestamp() <= _timestamp)
      return;

    _timestamp = _group->timestamp();

    const DeltaEngine::AssetData<T1>& d = _group->_datas[_key];
    _data = d._data;
    _state = d._state;

    if (!_data)
    {
      if (_data = _group->fallback(), _data)
      {
        if (_state == AssetState::Loading)
          _state = AssetState::LoadingFallback;
        else if (_state == AssetState::NotFound)
          _state = AssetState::NotFoundFallback;
        else
          _state = AssetState::NotLoadedFallback;
      }
      else if (_state != AssetState::Loading && _state != AssetState::NotFound)
        _state = AssetState::NotLoaded;
    }
  }

} // namespace DeltaEngine