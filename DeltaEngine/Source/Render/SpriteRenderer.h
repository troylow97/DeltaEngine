#pragma once

#include "Mesh.h"
#include "Core/Math/Transform.h"
#include "Core/Math/Color.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "../Physics/Collider.h" //for testing

namespace DeltaEngine
{
	class DE_API SpriteRenderer
	{
	public:
		Texture2D* sprite;
		Shader* shader;
		Transform transform;
		BoxCollider collider; //for testing
		CircleCollider collider2{}; //for testing
		Color color;

		Vector2 offset, tiling;

		SpriteRenderer();
		SpriteRenderer(std::string textureName);
		~SpriteRenderer();
		void Render(const Camera& camera);
	};
}
