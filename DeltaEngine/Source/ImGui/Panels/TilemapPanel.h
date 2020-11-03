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
		virtual float GetHeight() override;
		virtual float GetWidth() override;
		virtual ImVec2 GetTopLeft() override;
		virtual ImVec2 GetBottomRight() override;

	private:

		float height, width;
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}