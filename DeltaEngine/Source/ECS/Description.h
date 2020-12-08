/**********************************************************************************
* \file   Description.h
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
  struct ComponentMeta;

  struct Description
  {
    struct Details
    {
      const ComponentMeta* meta;
      size_t offset;
    };

    std::vector<Details> metalist;
    size_t capacity{0};
  };
} // namespace DeltaEngine
