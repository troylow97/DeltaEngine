/**********************************************************************************
* \file   ErrorCheck.h
* \brief  The file contains opengl specific asserts.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <GL/glew.h>
#include "Core/Debugging/Assert.h"

namespace DeltaEngine
{
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

  void GLClearError();

  bool GLLogCall(const char* function, const char* file, int line);
}
