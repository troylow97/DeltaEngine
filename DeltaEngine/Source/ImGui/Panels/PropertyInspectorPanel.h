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
	private:
		int number_of_lines;
		int number_of_empty_lines;
		unsigned int number_of_frames;
		bool m_texture_is_animation;
		std::string fileName;
		std::string stringcount;
	};
}