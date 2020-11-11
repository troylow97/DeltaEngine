#pragma once

#include <memory>
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
		std::unique_ptr<std::string> strpath;
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}