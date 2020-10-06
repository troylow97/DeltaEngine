#pragma once
#include "Camera.h"
#include "ECS/Entities.h"

namespace DeltaEngine
{
	// interface class Renderer
	class Renderer
	{
	public:
		Shader* shader;
		Transform transform;
		Color color;

		EntityID id;
		
		virtual ~Renderer() = default;
		virtual void Render(const Camera& camera) = 0;
	};
}
