#pragma once

#include "Mesh.h"
#include "Core/Math/Transform.h"
#include "Core/Math/Color.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Physics/Collider.h"

namespace DeltaEngine
{
	class SpriteRenderer
	{
	public:
		Texture2D* sprite;
		Shader* shader;
		Transform transform;
		Color color;
		BoxCollider collider;
		Vector2 offset, tiling;

		SpriteRenderer();
		SpriteRenderer(std::string textureName);
		~SpriteRenderer();
		void Render(const Camera& camera);
	};
}
