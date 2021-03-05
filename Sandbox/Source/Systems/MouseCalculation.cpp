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


namespace DeltaEngine
{
  namespace MouseCalculation
  {
    // shoot right if mouse is on the right
    bool ShootRight()
    {
      auto& player_id = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());

      FlipShooting();

      if (IsMouseOnRight())// && IsWithinRange(true)) // mouse on right side of player
      {
        if (AITools::isFacingRight(player_id)) // player facing right
        {
          return true;
        }
        return false;
      }
      return false;
    }
    
    // shoot left if mouse is on the left
    bool ShootLeft()
    {
      auto& player_id = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());

      FlipShooting();
      
      if (!IsMouseOnRight())// && IsWithinRange(false)) // mouse on left side of player
      {
        if (AITools::isFacingLeft(player_id)) // player facing left
        {
          return true;
        }
        return false;
      }
      return false;
    }

    // check if the mouse is on the right of player, else return false (mouse is on the left of player)
    bool MouseCalculation::IsMouseOnRight()
    {
      auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
#ifdef DE_EDITOR
      auto p_x = GamePanel::curr_mouse.point_x;
#else
      auto p_x = CalculateScreenCoordinate().x;
#endif
      if (p_x > player_pos.position.x)
        return true;
      return false;
    }

    // check if the mouse is on the top of player, else return false (mouse is below the player)
    bool MouseCalculation::IsMouseOnTop()
    {
      auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
#ifdef DE_EDITOR
      auto p_y = GamePanel::curr_mouse.point_y;
#else
      auto p_y = CalculateScreenCoordinate().y;
#endif
      if (p_y > player_pos.position.y)
        return true;
      return false;
    }

    // check if the shooting is within range
    bool MouseCalculation::IsWithinRange(bool right)
    {
      if (right == true)
      {
        float dot_product = 1.0f * CalculateDirectionVector().x + 0.0f * CalculateDirectionVector().y; // dot product between [x1, y1] and [x2, y2], x1*x2 + y1*y2
        float determinant = 1.0f * CalculateDirectionVector().y - 0.0f * CalculateDirectionVector().x;
        float angle = atan2(determinant, dot_product);
        angle *= (180.0f / Math::pi);// 3.141592653589793238463f);
        
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
        angle *= (180.0f / Math::pi);// 3.141592653589793238463f);
        
        if (angle > -45.0f && angle < 45.0f)
        {
          return true;
        }
        return false;
      }
      return false;
    }

    // gets the mouse position relative to the game panel
    Vector2 MouseCalculation::CalculateGameCoordinate()
    {
      auto p_x = GamePanel::curr_mouse.point_x;
      auto p_y = GamePanel::curr_mouse.point_y;
      
      return Vector2{ p_x, p_y };
    }

    // gets the mouse position relative to the screen
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

    // get the direction vector for the player to shoot
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

    Vector2 MouseCalculation::CalculateDirectionVectorToShoot()
    {
      // facing right
      if (IsWithinRange(true)) 
      {
        return CalculateDirectionVector();
      }
      // facing right but out of range
      if (IsWithinRange(true) == false) 
      {
        if (IsMouseOnTop())
        {
          Vector2 direction_to_shoot = { 1.0f / sqrtf(2.0f), 1.0f / sqrtf(2.0f) };
          return direction_to_shoot;
        }
        else
        {
          Vector2 direction_to_shoot = { 1.0f / sqrtf(2.0f), -1.0f / sqrtf(2.0f) };
          return direction_to_shoot;
        }
      }
      // facing left
      if (IsWithinRange(false)) 
      {
        return CalculateDirectionVector();
      }
      // facing left but out of range
      if (IsWithinRange(false) == false) 
      {
        if (IsMouseOnTop())
        {
          Vector2 direction_to_shoot = { -1.0f / sqrtf(2.0f), 1.0f / sqrtf(2.0f) };
          return direction_to_shoot;
        }
        else
        {
          Vector2 direction_to_shoot = { -1.0f / sqrtf(2.0f), -1.0f / sqrtf(2.0f) };
          return direction_to_shoot;
        }
      }
      return Vector2{ 0.0f, 0.0f };
    }

    // flip the player's direction of shooting if mouse moves to the other side of the player
    void MouseCalculation::FlipShooting()
    {
      auto& player_id = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());
      auto& player_image = env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(UnitManager::GetPlayerID());
      if (!IsMouseOnRight() && IsWithinRange(false))
      {
        if (AITools::isFacingRight(player_id))
        {
          player_image.m_FlipX = true;
        }
      }
      if (IsMouseOnRight() && IsWithinRange(true))
      {
        if (AITools::isFacingLeft(player_id))
        {
          player_image.m_FlipX = false;
        }
      }
    }
  }
}