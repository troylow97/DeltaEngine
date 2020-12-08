/**********************************************************************************
* \file   OpenGLSystem.h
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

#include "DEpch.h"
#include <GL/glew.h>

namespace DeltaEngine
{
  namespace RenderModule
  {
    class OpenGLSystem
    {
      HDC m_windowDC; //global Window-device context
      HGLRC m_wglDC; //OpenGL-device context

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

    extern OpenGLSystem* openGLSystem;
  }
}
