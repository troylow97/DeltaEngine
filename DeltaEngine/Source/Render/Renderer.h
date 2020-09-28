#pragma once
#include "Camera.h"

namespace DeltaEngine
{
	// interface class Renderer
	class Renderer
	{
	public:
		Shader* shader;
		Transform transform;
		Color color;
		
		virtual ~Renderer() = default;
		virtual void Render(const Camera& camera) = 0;
	};
}
