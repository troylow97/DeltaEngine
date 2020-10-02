#pragma once

#include "Mesh.h"
#include "Core/Math/Transform.h"
#include "Core/Math/Color.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"

namespace DeltaEngine
{
	class SpriteRenderer : public Renderer
	{
	public:
		Texture2D* sprite;

		Vector2 offset, tiling;

		SpriteRenderer(Texture2D* t, Shader* s);
		~SpriteRenderer();
		void Render(const Camera& camera) override;
	};
}
