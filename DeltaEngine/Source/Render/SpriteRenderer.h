#pragma once

#include "Mesh.h"
#include "Math/Transform.h"
#include "Math/Color.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "../Physics/Collider.h" //for testing

namespace DeltaEngine
{
	class DE_API SpriteRenderer
	{
	public:
		Mesh* mesh;
		Texture2D* sprite;
		Texture2D* spriteDist;
		Shader* shader;
		Transform transform;
		BoxCollider collider; //for testing
		CircleCollider collider2{}; //for testing
		Color color;
		SpriteRenderer();
		~SpriteRenderer();
		void Render(const Camera& camera);
	};
}
