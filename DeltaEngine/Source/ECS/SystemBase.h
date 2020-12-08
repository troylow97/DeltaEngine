/**********************************************************************************
* \file   SystemBase.h
* \brief  This file contains the definition of the abstract base class for all systems
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
  class SystemBase
  {
  protected:
    class EntityManager& em;

  public:
    class Query e_query;

    explicit SystemBase(EntityManager& ref) : em{ref}
    {
    }

    virtual void Initialize()
    {
    }

    virtual void Update() = 0;

    virtual void LateUpdate() = 0;

    virtual void Shutdown()
    {
    }

    virtual ~SystemBase() = default;
  };
} // namespace DeltaEngine
