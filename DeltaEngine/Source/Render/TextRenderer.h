#pragma once

#include "Font.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"

namespace DeltaEngine
{
	class TextRenderer : public Renderer
	{
		unsigned int VAO, VBO;
	public:
		Font* font;

		std::string text;
		TextRenderer(Font* f = nullptr, Shader* s = nullptr);
		~TextRenderer();
		void Render(const Camera& camera) override;
	};
}
