/**********************************************************************************
* \file   Find.h
* \brief  This file contains the implementation of typelist find functionality
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

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
