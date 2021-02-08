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
      auto& player_id = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());
      
      if (IsMouseOnRight() && IsWithinRange(true)) // mouse on right side of player
      {
        if (AITools::isFacingRight(player_id)) // player facing right
        {
          return true;
        }
        return false;
      }

      return false;
    }
    
    bool ShootLeft()
    {
      auto& player_id = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());
      
      if (!IsMouseOnRight() && IsWithinRange(false)) // mouse on left side of player
      {
        if (AITools::isFacingLeft(player_id)) // player facing left
        {
          return true;
        }
        return false;
      }
      return false;
    }
  
    bool MouseCalculation::IsMouseOnRight()
    {
      auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
#ifdef DE_EDITOR
      auto p_x = GamePanel::curr_mouse.point_x;
      auto p_y = GamePanel::curr_mouse.point_y;
#else
      auto p_x = CalculateScreenCoordinate().x;
      auto p_y = CalculateScreenCoordinate().y;
#endif
      if (p_x > player_pos.position.x)
        return true;
      return false;
    }

    bool MouseCalculation::IsWithinRange(bool right)
    {
      if (right == true)
      {
        float dot_product = 1.0f * CalculateDirectionVector().x + 0.0f * CalculateDirectionVector().y; // dot product between [x1, y1] and [x2, y2], x1*x2 + y1*y2
        float determinant = 1.0f * CalculateDirectionVector().y - 0.0f * CalculateDirectionVector().x;
        float angle = atan2(determinant, dot_product);
        angle *= (180.0 / 3.141592653589793238463);
        
        if (angle > -45.0f && angle < 45.0f)
        {
          return true;
        }
        return false;
      }
      if (right == false)
      {
        float dot_product = -1.0f * CalculateDirectionVector().x + 0.0f * CalculateDirectionVector().y; // dot product between [x1, y1] and [x2, y2], x1*x2 + y1*y2
        float determinant = -1.0f * CalculateDirectionVector().y - 0.0f * CalculateDirectionVector().x;
        float angle = atan2(determinant, dot_product);
        angle *= (180.0 / 3.141592653589793238463);
        
        if (angle > -45.0f && angle < 45.0f)
        {
          return true;
        }
        return false;
      }
      return false;
    }

    Vector2 MouseCalculation::CalculateDirectionVector()
    {
      auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
#ifdef DE_EDITOR
      auto p_x = CalculateGameCoordinate().x;
      auto p_y = CalculateGameCoordinate().y;
#else
      auto p_x = CalculateScreenCoordinate().x;
      auto p_y = CalculateScreenCoordinate().y;
#endif
      Vector2 direction_vector = { p_x - player_pos.position.x, p_y - player_pos.position.y };
      float magnitude = direction_vector.Magnitude();
      Vector2 normalized_direction_vector = { direction_vector.x / magnitude, direction_vector.y / magnitude };

      return normalized_direction_vector;
    }

    Vector2 MouseCalculation::CalculateGameCoordinate()
    {
      auto p_x = GamePanel::curr_mouse.point_x;
      auto p_y = GamePanel::curr_mouse.point_y;
      
      return Vector2{ p_x, p_y };
    }

    Vector2 MouseCalculation::CalculateScreenCoordinate()
    {
      auto& t = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>({ 0 });
      float cameraWidth = Camera::allCameras[0]->Max(t).x - Camera::allCameras[0]->Min(t).x;
      float cameraHeight = Camera::allCameras[0]->Max(t).y - Camera::allCameras[0]->Min(t).y;
      Vector2 screen_coordinate_mid;
      Vector2 screen_coordinate_min;
      Vector2 screen_coordinate_max;
      env.pECS->GetWorld().GetEntityManager().ForEach([&](Transform& t, Camera& c)
      {
        screen_coordinate_mid = { t.position.x ,t.position.y }; 
        screen_coordinate_min = { screen_coordinate_mid.x - (cameraWidth / 2), screen_coordinate_mid.y - (cameraHeight / 2) };  
        screen_coordinate_max = { screen_coordinate_mid.x + (cameraWidth / 2), screen_coordinate_mid.y + (cameraHeight / 2) }; 
      });
      float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().point_x;
      float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().point_y;
      auto p_x = ((cursorViewPortDistanceX / GetEnv().pWin->ClientRect().point_x) * cameraWidth) + screen_coordinate_min.x;
      auto p_y = screen_coordinate_max.y - ((cursorViewPortDistanceY / GetEnv().pWin->ClientRect().point_y) * cameraHeight);
      
      return Vector2{ p_x, p_y };
    }
  }
}
