#pragma once

#include "ImGui/Panels/IPanel.h"

namespace rttr {
	class instance;
}

namespace DeltaEngine
{
	class PropertyInspectorPanel : public IPanel
	{
	public:
		PropertyInspectorPanel(std::string str);
		~PropertyInspectorPanel();
		virtual bool Render(bool) override;
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