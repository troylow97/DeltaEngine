/**********************************************************************************
* \file   Details.h
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

namespace DeltaEngine::Types::Internal
{
  template <typename U, typename... Us>
  struct Details;

  template <typename U, typename... Us>
  struct Details<U, U, Us...>
  {
    static constexpr size_t index = 0;
  };

  template <typename U, typename V, typename... Us>
  struct Details<U, V, Us...>
  {
    static constexpr size_t index = (Details<U, Us...>::index) + 1;
  };
} // namespace DeltaEngine::Types::Internal
