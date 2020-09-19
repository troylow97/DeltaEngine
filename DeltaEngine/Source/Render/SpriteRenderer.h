#pragma once

#include "Mesh.h"
#include "Math/Transform.h"
#include "Math/Color.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

namespace DeltaEngine
{
	class SpriteRenderer
	{
	public:
		Texture2D* sprite;
		Shader* shader;
		Transform transform;
		Color color;

		Vector2 offset, tiling;

		SpriteRenderer();
		SpriteRenderer(std::string textureName);
		~SpriteRenderer();
		void Render(const Camera& camera);
	};
}
