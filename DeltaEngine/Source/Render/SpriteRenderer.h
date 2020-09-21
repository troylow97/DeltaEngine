#pragma once

#include "Mesh.h"
#include "Core/Math/Transform.h"
#include "Core/Math/Color.h"
#include "Texture.h"
#include "Shader.h"

namespace DeltaEngine
{
	class SpriteRenderer
	{
	public:
		Mesh* mesh;
		Texture2D* sprite;
		Shader* shader;
		Transform transform;
		Color color;
		SpriteRenderer();
		~SpriteRenderer();
		void Update();
	};
}
