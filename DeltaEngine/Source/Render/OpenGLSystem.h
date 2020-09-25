#pragma once

#include "DE_API.h"
#include <GL/glew.h>
#include "SpriteRenderer.h"
#include "ParticleSystem.h"

namespace DeltaEngine
{
	namespace RenderModule
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
			void TestRender(std::vector<SpriteRenderer*>, std::vector<ParticleSystem*>);
			void Exit();
		};
	}
}
