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
		virtual bool Render(bool) override;
		virtual ImVec2 GetTopLeft() override;
		virtual ImVec2 GetBottomRight() override;

	private:
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}