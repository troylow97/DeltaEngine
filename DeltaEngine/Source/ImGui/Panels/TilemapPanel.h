#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
	class TilemapPanel : public IPanel
	{
	public:
		TilemapPanel(std::string str);
		~TilemapPanel();
		bool DraggedFileIn();
		virtual bool Render(bool) override;
		virtual ImVec2 GetTopLeft() override;
		virtual ImVec2 GetBottomRight() override;

	private:
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}