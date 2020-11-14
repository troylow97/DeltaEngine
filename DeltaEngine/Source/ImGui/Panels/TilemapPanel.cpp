#include "ImGui/Panels/TilemapPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "Render/Texture.h"
#include "Assets/AssetManager.h"
#include "ECS/ECSModule.h"
#include "Core/Utils/FileUtils.h"

namespace DeltaEngine
{
    TilemapPanel::TilemapPanel(std::string str) :
        IPanel(str)
    {
      m_enabled = true;

    }

    TilemapPanel::~TilemapPanel()
    {
        m_enabled = false;
    }

    bool TilemapPanel::DraggedFileIn()
    {
        if (InputManager::Instance().CurrentPosition().point_x >= GetTopLeft().x && InputManager::Instance().CurrentPosition().point_x <= GetBottomRight().x
            && InputManager::Instance().CurrentPosition().point_y >= GetTopLeft().y && InputManager::Instance().CurrentPosition().point_y <= GetBottomRight().y)
        {
            std::cout << "it is in tileset panel!!!" << std::endl;
            return true;
        }
        return false;
    }

    void TilemapPanel::Render(bool isdragged)
    {
        ImGui::Begin( m_name.c_str(), &m_enabled/*, ImGuiWindowFlags_HorizontalScrollbar*/ );

        topLeft = ImGui::GetWindowContentRegionMin();
        bottomRight = ImGui::GetWindowContentRegionMax();

        topLeft.x += ImGui::GetWindowPos().x;
        topLeft.y += ImGui::GetWindowPos().y;
        bottomRight.x += ImGui::GetWindowPos().x;
        bottomRight.y += ImGui::GetWindowPos().y;

        //std::cout << "x is " << InputManager::Instance().CurrentPosition().point_x << " y is " << InputManager::Instance().CurrentPosition().point_y << std::endl;
        //std::cout << "render                   topLeft is " << topLeft.x << ", " << topLeft.y << std::endl;
        //std::cout << "render                   bottomRight is " << bottomRight.x << ", " << bottomRight.y << std::endl;

        ImGui::Text("Current tiles available:");

        if (isdragged)
        {
            DraggedFileIn();
        }
        std::string path = "Tilemap/";
        ImGuiStyle& style = ImGui::GetStyle();
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for (const auto& entry : std::filesystem::directory_iterator(path))
        { 
            if (entry.path().extension() == ".png")
            {
                std::string filePath = entry.path().string();

                std::size_t index = filePath.find_last_of("/");
                std::string fileName;
                for (size_t i = index + 1; i < filePath.length(); ++i)
                {
                    fileName += filePath[i]; // get fileName as XXX.png
                }

                // only files that end with .png will be loaded as texture 
                if (tileInfo.find(fileName) == tileInfo.end())
                {
                    tileInfo.insert(std::pair<std::string, std::string>(fileName, filePath));
                    env.pManager->Load<Texture2D>(fileName, filePath);
                    tileInfo[fileName] = env.pManager->Get<Texture2D>(fileName);

                    //size_t n = std::count(fileName.begin(), fileName.end(), '.');
                    //size_t temp_position = fileName.find_last_of(".png");
                    //
                    //if (temp_position == std::string::npos || n > 1)
                    //{
                    //    continue;
                    //}
                    //else
                    //{
                    //    tileInfo.insert(std::pair<std::string, std::string>(fileName, filePath));
                    //    env.pManager->Load<Texture2D>(fileName, filePath);
                    //    tileInfo[fileName] = env.pManager->Get<Texture2D>(fileName);
                    //}
                }

                uint64_t textureID;
                Sprite _sprite = { fileName, 0 };

                // setting all png files as ImageButton 
                textureID = _sprite.GetTexture()->GetRendererID();
                ImGui::PushID(textureID);
                if (ImGui::ImageButton(reinterpret_cast<void*>(textureID),
                    ImVec2{ 32,32 },
                    ImVec2{ _sprite.GetOffset().x, _sprite.GetOffset().y },
                    ImVec2{ _sprite.GetOffset().x + _sprite.GetTiling().x, _sprite.GetOffset().y + _sprite.GetTiling().y }))
                {
                    //std::cout << "clicking tiles" << std::endl;
                };
                float last_tile_x2 = ImGui::GetItemRectMax().x;
                float next_tile_x2 = last_tile_x2 + style.ItemSpacing.x + 32.0; // Expected position if next tile was on same line
                if (next_tile_x2 < window_visible_x2)
                    ImGui::SameLine();
                ImGui::PopID();

                ImGuiDragDropFlags src_flags = 0;
                src_flags |= ImGuiDragDropFlags_SourceNoDisableHover; // Keep the source displayed as hovered
                src_flags |= ImGuiDragDropFlags_SourceAllowNullID;    // Allow items such as Text(), Image() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear imgui ecosystem and so we made it explicit

                if (ImGui::BeginDragDropSource(src_flags))
                {
                    ImGui::SetDragDropPayload("TILES", &textureID, sizeof(int));
                    InputManager::Instance().SetTilesetDragged(true);
                    //std::cout << "dragging tiles" << std::endl;
                    // display preview (haven't decided whether to display the filename or preview the texture)
                    ImGui::Image(reinterpret_cast<void*>(textureID),
                        ImVec2{ 32,32 },
                        ImVec2{ _sprite.GetOffset().x, _sprite.GetOffset().y },
                        ImVec2{ _sprite.GetOffset().x + _sprite.GetTiling().x, _sprite.GetOffset().y + _sprite.GetTiling().y });
                    ImGui::Text(fileName.c_str());
                    ImGui::EndDragDropSource();
                }
            }
        }

        if (ImGui::BeginDragDropTarget())
        {
            ImGuiDragDropFlags target_flags = 0;

            const ImGuiPayload* assetpayload = ImGui::AcceptDragDropPayload("ASSETFILES", target_flags);
            if (assetpayload)
            {
                std::string assetpayload_n = *(std::string*)(assetpayload->Data);
                std::wstring assetpayload_nws(assetpayload_n.begin(), assetpayload_n.end());
                std::size_t index = assetpayload_nws.find_last_of(L"/\\");
                std::wstring newFileName;
                std::wstring newPathName = L"Tilemap/";
                for (size_t i = index; i < assetpayload_nws.length(); ++i)
                {
                    newFileName += assetpayload_nws[i];
                }
                newPathName += newFileName;

                FileUtils::CopyFileW(assetpayload_nws, newPathName);
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::End();
    }
}
