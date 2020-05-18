#pragma once

#include <GL/glew.h>

namespace Graphics
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
		void SwapBuffers();
		void Exit();
	};
}
