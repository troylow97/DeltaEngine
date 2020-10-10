#pragma once
#include "Camera.h"
#include "ECS/Entities.h"

namespace DeltaEngine
{
	// interface class Renderer
	class DE_API Renderer
	{
	public:
		Shader* shader = nullptr;
		Transform transform;
		Color color;
		bool m_Active = true;

		virtual ~Renderer() = default;
		virtual void Render(const Camera& camera) = 0;
	};
}
