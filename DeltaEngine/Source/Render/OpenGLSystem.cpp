#include "DEpch.h"
#include "OpenGLSystem.h"
#include <iostream>
#include <algorithm>
#include "Window.h"
#include "../Core/Log.h"

namespace Graphics
{
	OpenGLSystem::OpenGLSystem()
		: m_wglDC{}, m_windowDC{}
	{

	}

	OpenGLSystem::~OpenGLSystem()
	{

	}

	void OpenGLSystem::Init()
	{
		InitializeRenderingEnvironment();

		glClearColor(1.0f, 0.2f, 0.2f, 1.0f);//RGBA

		if (glewInit() != GLEW_OK)
			DeltaEngine_CORE_ERROR("glewInit() failed!");
	}

	void OpenGLSystem::Update()
	{
		//update opengl
		glClear(GL_COLOR_BUFFER_BIT);

		RECT rect;
		GetClientRect(Window::mainHWND, &rect);
		glViewport((GLint)0, (GLint)0, rect.right - rect.left, rect.bottom - rect.top);
	}

	void OpenGLSystem::Exit()
	{
		CleanRenderingEnvironment();
		std::cout << "OpenGL system exited" << std::endl;
	}

	bool OpenGLSystem::InitializeRenderingEnvironment()
	{
		//create rendering window
		m_windowDC = GetDC(Window::mainHWND);

		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		BOOL b = EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &devMode);
		if (b == 0)
			return false;

		//drawing surface format
		PIXELFORMATDESCRIPTOR pfdesc;
		memset(&pfdesc, 0, sizeof(PIXELFORMATDESCRIPTOR));

		pfdesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfdesc.nVersion = 1;
		pfdesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
		pfdesc.iPixelType = PFD_TYPE_RGBA;
		pfdesc.cColorBits = (BYTE)devMode.dmBitsPerPel;//32; //24 bit color for front and back buffer
		pfdesc.cDepthBits = 24;//24 bit depth buffer - not used in this demo
		pfdesc.cStencilBits = 8; //8 bit stencil buffer - not used in this demo

		int pf = ChoosePixelFormat(m_windowDC, &pfdesc);//checks if the graphics card can support the pixel format requested
		if (pf == 0)
		{
			ReleaseDC(Window::mainHWND, m_windowDC);
			return false;
		}


		BOOL ok = SetPixelFormat(m_windowDC, pf, &pfdesc);
		if (!ok)
		{
			ReleaseDC(Window::mainHWND, m_windowDC);
			return false;
		}


		//set the OpenGL context
		m_wglDC = wglCreateContext(m_windowDC);
		if (!m_wglDC)
		{
			ReleaseDC(Window::mainHWND, m_windowDC);
			return false;
		}


		ok = wglMakeCurrent(m_windowDC, m_wglDC);
		if (!ok)
		{
			wglDeleteContext(m_wglDC);
			ReleaseDC(Window::mainHWND, m_windowDC);
			return false;
		}

		return true;
	}

	void OpenGLSystem::CleanRenderingEnvironment()
	{
		if (m_wglDC)
		{
			if (!wglMakeCurrent(NULL, NULL))
			{
				//log
			}
		}

		if (!wglDeleteContext(m_wglDC))
		{
			//log
		}
		m_wglDC = NULL;

		if (m_windowDC && !ReleaseDC(Window::mainHWND, m_windowDC))
		{
			m_windowDC = NULL;
		}
	}

	void OpenGLSystem::SwapBuffers()
	{
		::SwapBuffers(m_windowDC); //using double buffering
	}
}
