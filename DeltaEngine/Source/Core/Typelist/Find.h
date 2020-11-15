#pragma once
#include <type_traits>

namespace DeltaEngine::Types::Internal
{
  template <template <typename> typename Predicate, typename NotFound, typename... Ts>
  struct Find;

  template <template <typename> typename Predicate, typename NotFound>
  struct Find<Predicate, NotFound>
  {
    using type = NotFound;
  };

  template <template <typename> typename Predicate, typename NotFound, typename T, typename... Ts>
  struct Find<Predicate, NotFound, T, Ts...>
  {
    using type = std::conditional_t<Predicate<T>::value, T, typename Find<Predicate, NotFound, Ts...>::type>;
  };
}
