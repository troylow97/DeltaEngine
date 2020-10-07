#pragma once
#include "Camera.h"
#include "ECS/Entities.h"

namespace DeltaEngine
{
	// interface class Renderer
	class DE_API Renderer
	{
	public:
		Shader* shader;
		Transform transform;
		Color color;

		virtual ~Renderer() = default;
		virtual void Render(const Camera& camera) = 0;
	};
}
