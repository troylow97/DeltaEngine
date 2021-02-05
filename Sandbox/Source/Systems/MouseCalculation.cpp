/**********************************************************************************
* \file   MouseCalculation.cpp
* \brief  The file contains the system for doing the calculations for mouse related
*         attacks
* \author Chin, Clara,   100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "MouseCalculation.h"
#include "UnitManager.h"
#include "DeltaEngine.h"
#include "ECS/ECSModule.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/InputManager.h"
#include "AI/AITools.h"
#include "ImGui/Panels/GamePanel.h"

//#include <fstream>


namespace DeltaEngine
{
  namespace MouseCalculation
  {
    bool ShootRight()
    {
      auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
      auto& player_id = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());
      
      if (IsMouseOnRight()) // mouse on right side of player
      {
        //std::cout << "yes mouse is on the right side of the player" << std::endl;
        if (AITools::isFacingRight(player_id)) // player facing right
        {
          //std::cout << "AITools::isFacingRight(id) is " << AITools::isFacingRight(player_id) << std::endl;
          return true;
        }
        return false;
      }
      return false;
    }
    
    bool ShootLeft()
    {
      auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
      auto& player_id = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());
      
      if (!IsMouseOnRight()) // mouse on left side of player
      {
        //std::cout << "yes mouse is on the left side of the player" << std::endl;
        if (AITools::isFacingLeft(player_id)) // player facing left
        {
          //std::cout << "AITools::isFacingLeft(id) is " << AITools::isFacingRight(player_id) << std::endl;
          return true;
        }
        return false;
      }
      return false;
    }
  
    bool MouseCalculation::IsMouseOnRight()
    {
      //std::ofstream txtOut;
      //txtOut.open("checkmousethingy.txt");

      auto& t = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>({ 0 });
      float cameraWidth = Camera::allCameras[0]->Max(t).x - Camera::allCameras[0]->Min(t).x;
      float cameraHeight = Camera::allCameras[0]->Max(t).y - Camera::allCameras[0]->Min(t).y;
#ifdef DE_EDITOR
      auto p_x = GamePanel::curr_mouse.point_x;
      auto p_y = GamePanel::curr_mouse.point_y;
      auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());

      if (p_x > player_pos.position.x)
        return true;
#else
      float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().point_x;
      float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().point_y;
      auto p_x = ((cursorViewPortDistanceX / GetEnv().pWin->ClientRect().point_x) * cameraWidth) + Camera::allCameras[0]->Min(t).x;
      auto p_y = Camera::allCameras[0]->Max(t).y - ((cursorViewPortDistanceY / GetEnv().pWin->ClientRect().point_y) *cameraHeight);
      auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
      
      //if (txtOut.is_open())
      //{
      //  txtOut << "player_pos.position.x is " << player_pos.position.x << "\n";
      //  txtOut << "p_x is " << p_x << "\n";
      //  txtOut << "\n";
      //  txtOut << "player_pos.position.y is " << player_pos.position.y << "\n";
      //  txtOut << "p_y is " << p_y << "\n";
      //  txtOut << "\n";
      //  txtOut << "\n";
      //}
      
      if (p_x > player_pos.position.x)
        return true;
#endif
        return false;
    }

    Vector2 MouseCalculation::CalculateDirectionVector()
    {
      auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
#ifdef DE_EDITOR
      auto p_x = GamePanel::curr_mouse.point_x;
      auto p_y = GamePanel::curr_mouse.point_y;

      Vector2 direction_vector = { p_x - player_pos.position.x, p_y - player_pos.position.y }; // 1.06608, 0.592079
      float magnitude = direction_vector.Magnitude();
      Vector2 normalized_direction_vector = { direction_vector.x / magnitude, direction_vector.y / magnitude};
#else

#endif

      return normalized_direction_vector;
    }
  }
}
