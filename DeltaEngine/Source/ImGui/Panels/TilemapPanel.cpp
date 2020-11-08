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
        if (InputManager::Get()->CurrentPosition().point_x >= GetTopLeft().x && InputManager::Get()->CurrentPosition().point_x <= GetBottomRight().x
            && InputManager::Get()->CurrentPosition().point_y >= GetTopLeft().y && InputManager::Get()->CurrentPosition().point_y <= GetBottomRight().y)
        {
            std::cout << "it is in tileset panel!!!" << std::endl;
            return true;
        }
        return false;
    }

    void TilemapPanel::Render(bool isdragged)
    {
        ImGui::Begin(m_name.c_str(), &m_enabled);
        auto& em = env.pECS->GetWorld().GetEntityManager();

        topLeft = ImGui::GetWindowContentRegionMin();
        bottomRight = ImGui::GetWindowContentRegionMax();

        topLeft.x += ImGui::GetWindowPos().x;
        topLeft.y += ImGui::GetWindowPos().y;
        bottomRight.x += ImGui::GetWindowPos().x;
        bottomRight.y += ImGui::GetWindowPos().y;

        //std::cout << "x is " << InputManager::Get()->CurrentPosition().point_x << " y is " << InputManager::Get()->CurrentPosition().point_y << std::endl;
        //std::cout << "render                   topLeft is " << topLeft.x << ", " << topLeft.y << std::endl;
        //std::cout << "render                   bottomRight is " << bottomRight.x << ", " << bottomRight.y << std::endl;

        ImGui::Text("Current tiles available:");

        if (isdragged)
        {
            DraggedFileIn();
        }
        std::string path = "Tilemap/";

        for (const auto& entry : std::filesystem::directory_iterator(path))
        { 
            std::string filePath = entry.path().string();

            std::size_t index = filePath.find_last_of("/");
            std::string fileName;
            for (size_t i = index + 1; i < filePath.length(); ++i)
            {
                fileName += filePath[i];
            }

            if (tileInfo.find(fileName) == tileInfo.end())
            {
                size_t n = std::count(fileName.begin(), fileName.end(), '.');
                size_t temp_position = fileName.find_last_of(".png");

                if (temp_position == std::string::npos || n > 1)
                {
                    continue;
                }
                else
                {
                    tileInfo.insert(std::pair<std::string, std::string>(fileName, filePath));
                    env.pManager->Load<Texture2D>(fileName, filePath);
                    tileInfo[fileName] = env.pManager->Get<Texture2D>(fileName);
                }
            }
            
            std::vector<Sprite> spritelist;
            uint64_t textureID;
            Sprite _sprite = { fileName, 0 };
            spritelist.push_back(_sprite);

            for (int i = 0; i < spritelist.size(); ++i)
            {
                textureID = spritelist[i].GetTexture()->GetRendererID();

                if (ImGui::ImageButton(reinterpret_cast<void*>(textureID),
                    ImVec2{ 32,32 },
                    ImVec2{ spritelist[i].GetOffset().x, spritelist[i].GetOffset().y },
                    ImVec2{ spritelist[i].GetOffset().x + spritelist[i].GetTiling().x, spritelist[i].GetOffset().y + spritelist[i].GetTiling().y }))
                {
                    //static int textureClicked = 0;
                    //textureClicked++;
                    //std::cout << "textureclicked is " << textureClicked << std::endl;
                    //
                    //if (textureClicked & 1)
                    //{
                    //    //strcpy(str1, spritelist[i].GetName().c_str());
                    //}
                }
                ImGui::SameLine();
            }
        }

        ImGui::End();
    }

    ImVec2 TilemapPanel::GetTopLeft()
    {
        return topLeft;
    }

    ImVec2 TilemapPanel::GetBottomRight()
    {
        return bottomRight;
    }
}