#include "AnimationPanel.h"
#include "ImGui/IconsFontAwesome5.h"

#include "DeltaEngine.h"
#include "imgui.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "ImGui/ImGuiMath.h"
#include "Input/Keys.h"

#include <string>

namespace DeltaEngine
{
  AnimationPanel::AnimationPanel(std::string str)
    : IPanel(str)
  {
    m_enabled = true;
  }

  AnimationPanel::~AnimationPanel()
  {
    m_enabled = false;
  }

  void AnimationPanel::Render()
  {
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Animation Editor"))
    {
      static bool playingAnimation = false;
      static float animTimer = 0.f;

      ImGui::BeginChild("List of Frames", ImVec2(100, 0));
      float lineHeight = ImGui::GetIO().FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
      ImVec2 buttonSize = { lineHeight, lineHeight };
      if (ImGui::Button(ICON_FA_PLAY, buttonSize))
        playingAnimation = !playingAnimation;

      ImGui::Text("Key Frames");
      ImGui::Separator();

      struct Node
      {
        char m_Name[32];

        Node(const char* name)
        {
          strcpy_s(m_Name, name);
        }
      };
      // static vars
      static std::vector<Node> nodes;
      static bool loaded = false;
      static size_t nodeSelected = false;

      // non-static vars
      unsigned int highestKeyFrame = 0;

      AnimationClip* clip = GetEnv().pManager->Get<AnimationClip>(std::string(clipName));

      if (!loaded && clip)
      {
        int c = 0;
        for (auto& [FrameNumber, Value] : clip->m_Sprites)
        {
          if (highestKeyFrame < FrameNumber)
            highestKeyFrame = FrameNumber;

          nodes.push_back(Node("Frame"));
          nodes.push_back(Node("Sprite"));
          loaded = true;
          ++c;
        }
      }

      for (size_t i = 0; i < nodes.size(); ++i)
      {
        Node* node = &nodes[i];
        if (ImGui::Selectable(node->m_Name))
          nodeSelected = i;
        if (ImGui::IsItemHovered())
        {

        }
      }
      ImGui::EndChild();

      if (clip)
      {
        if (ImGui::BeginChild("Animation Previewer", ImVec2(100, 0)))
        {
          ImGui::SameLine();
          ImGui::Text("Preview");

          if (playingAnimation)
            animTimer += GetEnv().pClock->DeltaTime();
          while (animTimer > 1.0f * clip->GetTotalFrames() / clip->GetFps())
            animTimer -= 1.0f * clip->GetTotalFrames() / clip->GetFps();

          Sprite sprite = clip->m_Sprites[Math::Clamp(static_cast<unsigned>(animTimer * clip->GetFps()), 0, clip->GetTotalFrames() - 1)];

          if (sprite)
          {
            uint64_t textureID = sprite.GetTexture()->GetRendererID();
            static float zoom = 1.0f;
            if (ImGui::IsWindowHovered())
              if (ImGui::IsKeyDown(DEVK_LCTRL))
                zoom += 0.02f * ImGui::GetIO().MouseWheel;
            ImGui::Image(
              reinterpret_cast<void*>(textureID),
              ImVec2{ sprite.GetWidth() * zoom, sprite.GetHeight() * zoom },
              ImVec2{ sprite.GetOffset().x, sprite.GetOffset().y },
              ImVec2{ sprite.GetOffset().x + sprite.GetTiling().x, sprite.GetOffset().y + sprite.GetTiling().y },
              ImVec4{ 1, 1, 1, 1 }, ImVec4{ 1, 1, 1, 1 });
          }
          ImGui::EndChild();
        }
      }

      ImGui::End();
    }
  }
}
