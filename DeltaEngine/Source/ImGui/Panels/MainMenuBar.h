#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
	class MainMenuBar : public IPanel
	{
	public:
		MainMenuBar(std::string str);
		~MainMenuBar();
		virtual bool Render() override;
		//virtual bool DraggedFileIn() override;
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