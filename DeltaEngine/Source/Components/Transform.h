/**********************************************************************************
* \file   Transform.h
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
#include "Core/Math/DE_Math.h"

namespace DeltaEngine
{
  struct Transform
  {
    Vector3 position;
    Vector3 scale;
    Quaternion rotation;
    Transform();
    Transform(Vector3 position, Quaternion rotation, Vector3 scale);
    Matrix4x4 LocalToWorldMatrix();
  };
}
