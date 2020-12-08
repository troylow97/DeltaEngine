/**********************************************************************************
* \file   ActionList.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <list>
#include <functional>
#include <algorithm>

namespace Util
{
  template <typename... ActionArgs>
  using Action = std::function<void(ActionArgs ...)>;

  template <typename... ActionArgs>
  class ActionList
  {
    // Possible optimization by using unsigned int / reusing HandleID
    using HandleID = size_t;
    using Handle = std::pair<HandleID, Action<const ActionArgs&...>>;
    std::list<Handle> _handleList;
    HandleID _top{1ULL};

  public:
    ActionList() = default;

    ActionList(const ActionList&) = delete;
    ActionList& operator=(const ActionList&) = delete;

    ActionList(ActionList&&) = default;
    ActionList& operator=(ActionList&&) = default;

    ~ActionList() = default;

    HandleID Subscribe(Action<const ActionArgs&...> action)
    {
      _handleList.push_back({_top, action});
      return _top++;
    }

    void Unsubscribe(HandleID id)
    {
      auto result = std::find_if(_handleList.begin(),
                                 _handleList.end(),
                                 [id](const Handle& handle)
                                 {
                                   return handle.first == id;
                                 });

      if (result != _handleList.end())
        _handleList.remove_if(
          [id](const Handle& handle)
          {
            return handle.first == id;
          });
    }

    void Invoke(const ActionArgs&... args)
    {
      for (const Handle& ref : _handleList)
        ref.second(args...);
    }

    void Clear()
    {
      _handleList.clear();
      _top = 1ULL;
    }
  };
} // namespace Util
