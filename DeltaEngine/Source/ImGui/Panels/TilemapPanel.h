#pragma once

#include <map>
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
	class TilemapPanel : public IPanel
	{
	public:
		TilemapPanel(std::string str);
		~TilemapPanel();
		bool DraggedFileIn();
		void Render(bool) override;
		ImVec2 GetTopLeft() override;
		ImVec2 GetBottomRight() override;

	private:
		std::map < std::string, std::string >tileInfo;
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}