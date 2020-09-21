#pragma once

#include "DE_API.h"
#include <GL/glew.h>
#include "SpriteRenderer.h"

namespace DeltaEngine::RenderModule
{
	class DE_API OpenGLSystem
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
		void TestRender(std::vector<SpriteRenderer*>&);
		void SwapBuffers();
		void Exit();
	};
}
