#pragma once

#include "Mesh.h"
#include "Components/Transform.h"
#include "Core/Math/Color.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "Sprite.h"

namespace DeltaEngine
{
	class DE_API SpriteRenderer : public Renderer
	{
	public:
		Sprite sprite;

		Vector2 m_Offset, m_Tiling;

		bool m_FlipX, m_FlipY;

		bool m_Shaded, m_Wireframe;

		SpriteRenderer(Texture2D* t = nullptr, Shader* s = nullptr);
		~SpriteRenderer();
		void Render(const Camera& camera) override;
	};
}
