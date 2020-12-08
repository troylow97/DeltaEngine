/**********************************************************************************
* \file   EntityInfo.h
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
#include <string>
#include "Core/TypeAlias.h"

namespace DeltaEngine
{
  struct EntityName
  {
    std::string name{};
  };

  struct Parent
  {
    size_t p_id{u64_max};
  };
}
