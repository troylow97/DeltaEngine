/**********************************************************************************
* \file   OpenGLSystem.cpp
* \brief  The file contains implementation of OpenGLSystem class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "DEpch.h"
#include "OpenGLSystem.h"
#include "Window.h"
#include "Camera.h"
#include "Mesh.h"
#include "Font.h"
#include "Core/Debugging/Gizmos.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_opengl3.h>


#include "crtdbg.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

namespace DeltaEngine
{
  namespace RenderModule
  {
    OpenGLSystem* openGLSystem;

    OpenGLSystem::OpenGLSystem()
      : m_windowDC{}, m_wglDC{}
    {
    }

    OpenGLSystem::~OpenGLSystem()
    {
      DeltaEngine_CORE_INFO("OpenGL system deleted");
    }

    void OpenGLSystem::Init()
    {
      DeltaEngine_CORE_INFO("Initializing OpenGL...");
      //ImGui_ImplWin32_EnableDpiAwareness();
      InitializeRenderingEnvironment();

      glClearColor(0, 0, 0, 1); //RGBA

      glewExperimental = GL_TRUE;
      if (glewInit() != GLEW_OK)
      {
        DeltaEngine_CORE_ERROR("glewInit() failed!");
      }
      else
      {
        DeltaEngine_CORE_INFO("Initializing OpenGL successful");
        DeltaEngine_CORE_INFO("OpenGL version: {0}", glGetString( GL_VERSION ));
      }

      // Initialize common meshes
      Mesh::Init();
      Font::Init();
      Gizmos::Init();
#ifdef DE_EDITOR
      Camera* editorCam = new Camera(true);
      editorCam->backgroundColor = Color(71 / 255.0f, 71 / 255.0f, 71 / 255.0f, 1);
#endif
    }

    void OpenGLSystem::Update()
    {
      //update opengl
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT);

      RECT rect;
      GetClientRect(env.pWin->GetHandle(), &rect);
      glViewport(static_cast<GLint>(0), static_cast<GLint>(0), rect.right - rect.left, rect.bottom - rect.top);
    }

    void OpenGLSystem::Exit()
    {
      DeltaEngine_CORE_INFO( "Shutting down OpenGL..." );
#ifdef DE_EDITOR
      delete Camera::editorCamera;
#endif
      Gizmos::Exit();
      Font::Exit();
      Mesh::Exit();
      CleanRenderingEnvironment();
      DeltaEngine_CORE_INFO("Shutting down OpenGL successful");
    }

    HDC OpenGLSystem::GetWindowContext()
    {
      return m_windowDC;
    }

    HGLRC OpenGLSystem::GetGLContext()
    {
      return m_wglDC;
    }

    bool OpenGLSystem::InitializeRenderingEnvironment()
    {
      //create rendering window
      m_windowDC = GetDC(env.pWin->GetHandle());

      DEVMODE devMode = {0};
      devMode.dmSize = sizeof(DEVMODE);
      BOOL b = EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);
      if (b == 0)
        return false;

      //drawing surface format
      PIXELFORMATDESCRIPTOR pfdesc;
      memset(&pfdesc, 0, sizeof(PIXELFORMATDESCRIPTOR));

      pfdesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
      pfdesc.nVersion = 1;
      pfdesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
      pfdesc.iPixelType = PFD_TYPE_RGBA;
      pfdesc.cColorBits = static_cast<BYTE>(devMode.dmBitsPerPel); //32; //24 bit color for front and back buffer
      pfdesc.cDepthBits = 24; //24 bit depth buffer - not used in this demo
      pfdesc.cStencilBits = 8; //8 bit stencil buffer - not used in this demo

      int pf = ChoosePixelFormat(m_windowDC, &pfdesc);
      //checks if the graphics card can support the pixel format requested
      if (pf == 0)
      {
        ReleaseDC(env.pWin->GetHandle(), m_windowDC);
        return false;
      }


      BOOL ok = SetPixelFormat(m_windowDC, pf, &pfdesc);
      if (!ok)
      {
        ReleaseDC(env.pWin->GetHandle(), m_windowDC);
        return false;
      }


      //set the OpenGL context
      m_wglDC = wglCreateContext(m_windowDC);
      if (!m_wglDC)
      {
        ReleaseDC(env.pWin->GetHandle(), m_windowDC);
        return false;
      }


      ok = wglMakeCurrent(m_windowDC, m_wglDC);
      if (!ok)
      {
        wglDeleteContext(m_wglDC);
        ReleaseDC(env.pWin->GetHandle(), m_windowDC);
        return false;
      }

      return true;
    }

    void OpenGLSystem::CleanRenderingEnvironment()
    {
      if (m_wglDC)
      {
        if (!wglMakeCurrent(nullptr, nullptr))
        {
          //log
        }
      }

      if (!wglDeleteContext(m_wglDC))
      {
        //log
      }
      m_wglDC = nullptr;

      if (m_windowDC && !ReleaseDC(env.pWin->GetHandle(), m_windowDC))
      {
        m_windowDC = nullptr;
      }
    }
  }
}
