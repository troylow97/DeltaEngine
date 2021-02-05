/**********************************************************************************
* \file   AssetEnum.h
* \brief  This file contains the definition of the AssetEnum
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
  enum class AssetState : unsigned
  {
    NotLoaded,
    NotLoadedFallback,
    Loading,
    LoadingFallback,
    NotFound,
    NotFoundFallback,
    Mutable,
    Final
  };

  enum class AssetLifetime : unsigned
  {
    Persistent,
    ReferenceCounted,
    Managed
  };
}
