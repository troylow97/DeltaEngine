#include "SpriteEditorPanel.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Render/Texture.h"

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
		ImGui::Begin(m_name.c_str(), &m_enabled, ImGuiWindowFlags_MenuBar);

		topLeft = ImGui::GetWindowContentRegionMin();
		bottomRight = ImGui::GetWindowContentRegionMax();
		topLeft.x += ImGui::GetWindowPos().x;
		topLeft.y += ImGui::GetWindowPos().y;
		bottomRight.x += ImGui::GetWindowPos().x;
		bottomRight.y += ImGui::GetWindowPos().y;

		Texture2D* texture = GetEnv().pManager->Get<Texture2D>(textureName);
		uint64_t textureID = texture->GetRendererID();
    if (ImGui::BeginMenuBar())
    {
			if (ImGui::BeginMenu("Auto Slicing"))
			{
				static const char* components[]{ "automatic", "row by column" };
				static int selected = 0;
				ImGui::Combo("Slice Type", &selected, components, IM_ARRAYSIZE(components));
				if (ImGui::Button("Auto Slice"))
				{
					// Slice it
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
    }

		ImGui::Image(
			reinterpret_cast<void*>(textureID),
			ImVec2{ texture->GetWidth() * 0.1f, texture->GetHeight() * 0.1f },
			ImVec2{ 0, 0 }, ImVec2{ 1, 1 }, ImVec4{ 1, 1, 1, 1 }, ImVec4{ 1, 1, 1, 1 });

		ImGui::End();

	}

}
