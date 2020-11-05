#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
	class WorldPanel : public IPanel
	{
	public:
		WorldPanel(std::string str);
		~WorldPanel();
		bool DraggedFileIn();
		void Render(bool) override;
		ImVec2 GetTopLeft() override;
		ImVec2 GetBottomRight() override;

	private:
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}