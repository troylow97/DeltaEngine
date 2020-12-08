/**********************************************************************************
* \file   Remove.h
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

namespace DeltaEngine
{
  template <typename... Ts>
  struct Typelist;
}

namespace DeltaEngine::Types::Internal
{
  template <template <typename> typename Predicate, typename... Ts>
  struct Remove
  {
    template <typename U>
    struct Impl : public std::conditional_t<Predicate<U>::value, std::false_type, std::true_type>
    {
    };

    template <template <typename> typename Predicate, typename List, typename... Ts>
    struct Rebuilder;

    template <template <typename> typename Predicate, typename List>
    struct Rebuilder<Predicate, List>
    {
      using type = List;
    };

    template <template <typename> typename Predicate, typename List, typename T, typename... Ts>
    struct Rebuilder<Predicate, List, T, Ts...>
    {
      using type = std::conditional_t<Predicate<T>::value,
                                      typename Rebuilder<Predicate, typename List::template Append<T>, Ts...>::type,
                                      typename Rebuilder<Predicate, List, Ts...>::type>;
    };

    using type = typename Rebuilder<Impl, Typelist<>, Ts...>::type;
  };
} // namespace DeltaEngine::Types::Internal
