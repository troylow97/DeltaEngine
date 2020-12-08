/**********************************************************************************
* \file   Concatenate.h
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
  template <typename List, typename InputList>
  struct Concatenate;

  template <typename... ListTypes, typename... InputListTypes>
  struct Concatenate<Typelist<ListTypes...>, Typelist<InputListTypes...>>
  {
    using type = Typelist<ListTypes..., InputListTypes...>;
  };
}
