#pragma once
#include <list>
#include <functional>
#include <algorithm>

namespace DeltaEngine
{

  // I added this Alias inside TypeAlias.h
  // To be removed when merged
template <typename... Args>
using Action = std::function<void ( Args... )>;

  template <typename... ActionArgs>
  class ActionList
  {
    // Possible optimization by using unsigned int / reusing HandleID
    using HandleID = size_t;
    using Handle = std::pair<HandleID, Action<const ActionArgs &...>>;
    std::list<Handle> _handleList;
    HandleID _top{1ULL};

  public:
    ActionList() = default;

    ActionList(const ActionList &) = delete;
    ActionList &operator=(const ActionList &) = delete;

    ActionList(ActionList &&) = default;
    ActionList &operator=(ActionList &&) = default;

    ~ActionList() = default;

    HandleID Subscribe(Action<const ActionArgs &...> action)
    {
        _handleList.push_back({ _top, action });
      return _top++;
    }

    void Unsubscribe(HandleID id)
    {
      auto &result = std::find_if(_handleList.begin(),
                                  _handleList.end(),
                                  [id](const Handle &handle) {
                                    return handle.first == id;
                                  });

      if (result != _handleList.end())
        _handleList.remove_if(
            [id](const Handle &handle) { return handle.first == id; });
    }

    void Invoke(const ActionArgs&... args)
    {
      for (Handle& ref : _handleList)
        ref.second(args...);
    }

    void Clear()
    {
      _handleList.clear();
      _top = 1ULL;
    }
  };
} // namespace DeltaEngine
