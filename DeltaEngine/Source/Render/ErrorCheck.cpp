/**********************************************************************************
* \file   ErrorCheck.cpp
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
#include "ErrorCheck.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
  void GLClearError()
  {
    while (glGetError() != GL_NO_ERROR);
  }

  bool GLLogCall(const char* function, const char* file, int line)
  {
    while (GLenum error = glGetError())
    {
      DeltaEngine_CORE_ERROR("OpenGL error code {}: {} {}:{}", error, function, file, line);
      return false;
    }
    return true;
  }
}