#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
	class ViewportPanel : public IPanel
	{
	public:
		ViewportPanel(std::string str);
		~ViewportPanel();
		bool DraggedFileIn();
    void Render(bool) override;
		ImVec2 GetTopLeft() override;
		ImVec2 GetBottomRight() override;

	private:
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}