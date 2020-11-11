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
		void Render(bool) override;
		//virtual bool DraggedFileIn() override;
	};
}