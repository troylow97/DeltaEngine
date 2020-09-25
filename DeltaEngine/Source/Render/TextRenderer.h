#pragma once

#include "Font.h"
#include "Shader.h"
#include "Camera.h"

namespace DeltaEngine
{
	class TextRenderer
	{
		unsigned int VAO, VBO;
	public:
		Font* font;
		Shader* shader;
		Transform transform;
		Color color;
		std::string text;
		TextRenderer();
		void Render(const Camera& camera);
	};
}
