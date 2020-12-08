/**********************************************************************************
* \file   Concatenate.h
* \brief  This file contains the implementation for typelist concatenation
*
* \author Tan, Tong Wee, 100% Code Contribution
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
  template <typename List, typename InputList>
  struct Concatenate;

  template <typename... ListTypes, typename... InputListTypes>
  struct Concatenate<Typelist<ListTypes...>, Typelist<InputListTypes...>>
  {
    using type = Typelist<ListTypes..., InputListTypes...>;
  };
}
