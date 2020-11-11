#include "SpriteEditorPanel.h"

namespace DeltaEngine
{
	SpriteEditorPanel::SpriteEditorPanel(std::string str)
		: IPanel(str)
	{
		m_enabled = true;
	}
	SpriteEditorPanel::~SpriteEditorPanel()
	{
		m_enabled = false;
	}
	void SpriteEditorPanel::Render(bool)
	{
		ImGui::Begin(m_name.c_str(), &m_enabled);
		topLeft = ImGui::GetWindowContentRegionMin();
		bottomRight = ImGui::GetWindowContentRegionMax();

    if (ImGui::BeginMainMenuBar())
    {

			ImGui::EndMainMenuBar();
    }

		ImGui::End();

	}

}
