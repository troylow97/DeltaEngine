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
