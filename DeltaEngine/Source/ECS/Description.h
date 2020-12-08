/**********************************************************************************
* \file   Description.h
* \brief  This file contains the definition of Description struct that contains
*         information of all the components
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
