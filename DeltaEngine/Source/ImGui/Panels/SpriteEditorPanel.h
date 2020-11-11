#pragma once

#include "DEpch.h"
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
	class SpriteEditorPanel : public IPanel
	{
	public:
		SpriteEditorPanel(std::string str);
		~SpriteEditorPanel();
		void Render(bool) override;

	private:
		std::string textureName;
	};
}
