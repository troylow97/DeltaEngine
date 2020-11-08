#pragma once

#include "DEpch.h"
#include <GL/glew.h>

namespace DeltaEngine
{
namespace RenderModule
{
class OpenGLSystem
{
  HDC m_windowDC;//global Window-device context
  HGLRC m_wglDC;//OpenGL-device context

  bool InitializeRenderingEnvironment();
  void CleanRenderingEnvironment();

public:
  OpenGLSystem();
  ~OpenGLSystem();

  void Init();
  void Update();
  void Exit();

  HDC GetWindowContext();
  HGLRC GetGLContext();
};
extern OpenGLSystem *openGLSystem;
}
}
