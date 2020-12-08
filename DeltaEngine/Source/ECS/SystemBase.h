/**********************************************************************************
* \file   SystemBase.h
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
