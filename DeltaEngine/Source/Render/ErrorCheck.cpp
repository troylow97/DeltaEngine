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
