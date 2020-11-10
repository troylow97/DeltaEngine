#pragma once

#include <map>
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
	class SpriteEditorPanel : public IPanel
	{
	public:
		SpriteEditorPanel(std::string str);
		~SpriteEditorPanel();
		void Render(bool) override;
		ImVec2 GetTopLeft() override;
		ImVec2 GetBottomRight() override;

	private:
		std::map < std::string, std::string >tileInfo;
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}
