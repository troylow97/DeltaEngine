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
		Texture2D* spriteDist;
		Shader* shader;
		Transform transform;
		Color color;
		SpriteRenderer();
		~SpriteRenderer();
		void Render(const Camera& camera);
	};
}
