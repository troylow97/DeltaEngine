#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
	class AssetPanel : public IPanel
	{
	public:
		AssetPanel(std::string str);
		~AssetPanel();
		bool DraggedFileIn();
    void Render(bool) override;
		ImVec2 GetTopLeft() override;
		ImVec2 GetBottomRight() override;

	private:
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}