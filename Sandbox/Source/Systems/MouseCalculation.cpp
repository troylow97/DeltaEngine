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
        bool ShootRight()
        {
            auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
            auto& player_id = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());

            if (IsMouseOnRight()) // mouse on right side of player
            {
                std::cout << "yes mouse is on the right side of the player" << std::endl;
                if (AITools::isFacingRight(player_id)) // player facing right
                {
                    std::cout << "AITools::isFacingRight(id) is " << AITools::isFacingRight(player_id) << std::endl;
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
                std::cout << "yes mouse is on the left side of the player" << std::endl;
                if (AITools::isFacingLeft(player_id)) // player facing left
                {
                    std::cout << "AITools::isFacingLeft(id) is " << AITools::isFacingRight(player_id) << std::endl;
                    return true;
                }
                return false;
            }
            return false;
        }

        bool MouseCalculation::IsMouseOnRight()
        {
            ///////////////  ////// UI system
            /////////////////  prev_mouse = curr_mouse;
            /////////////////  auto& t = em.GetComponent<Transform>({0});
            /////////////////  float cameraWidth = Camera::allCameras[0]->Max(t).x - Camera::allCameras[0]->Min(t).x;
            /////////////////  float cameraHeight = Camera::allCameras[0]->Max(t).y - Camera::allCameras[0]->Min(t).y;
            /////////////////
            /////////////////#ifdef DE_EDITOR
            /////////////////  float cursorViewPortDistanceX = GamePanel::mouse_pos.x - GamePanel::render_pos.x;
            /////////////////  float cursorViewPortDistanceY = GamePanel::mouse_pos.y - GamePanel::render_pos.y;
            /////////////////
            /////////////////  curr_mouse.point_x = ( ( cursorViewPortDistanceX / GamePanel::render_size.x ) * cameraWidth ) + Camera::allCameras[0]->Min( t ).x;
            /////////////////  curr_mouse.point_y = Camera::allCameras[0]->Max( t ).y - ( ( cursorViewPortDistanceY / GamePanel::render_size.y ) * cameraHeight );
            /////////////////#else
            /////////////////
            /////////////////  float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().
            /////////////////    point_x;
            /////////////////  float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().
            /////////////////    point_y;
            /////////////////  auto p_x = ((cursorViewPortDistanceX / GetEnv().pWin->ClientRect().point_x) * cameraWidth) + Camera::allCameras[0]->
            /////////////////    Min(t).x;
            /////////////////  auto p_y = Camera::allCameras[0]->Max(t).y - ((cursorViewPortDistanceY / GetEnv().pWin->ClientRect().point_y) *
            /////////////////    cameraHeight);
            /////////////////#endif
            ///////////////    
            ///////////////  
            ///////////////    prev_mouse = curr_mouse;
            ///////////////    auto& t = em.GetComponent<Transform>({ 0 });
            ///////////////    float cameraWidth = Camera::allCameras[0]->Max(t).x - Camera::allCameras[0]->Min(t).x;
            ///////////////    float cameraHeight = Camera::allCameras[0]->Max(t).y - Camera::allCameras[0]->Min(t).y;
            ///////////////#ifdef DE_EDITOR
            ///////////////    float cursorViewPortDistanceX = GamePanel::mouse_pos.x - GamePanel::render_pos.x;
            ///////////////    float cursorViewPortDistanceY = GamePanel::mouse_pos.y - GamePanel::render_pos.y;
            ///////////////    curr_mouse.point_x = ((cursorViewPortDistanceX / GamePanel::render_pos.x) * cameraWidth) + Camera::allCameras[0]->Min(Camera::editorCameraTransform).x;
            ///////////////    curr_mouse.point_y = Camera::allCameras[0]->Max(Camera::editorCameraTransform).y - ((cursorViewPortDistanceY / GamePanel::render_pos.y) * cameraHeight);
            ///////////////    //float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GamePanel::render_pos.x;
            ///////////////    //float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GamePanel::render_pos.y;
            ///////////////    //auto p_x = ((cursorViewPortDistanceX / GamePanel::render_size.x) * cameraWidth) + Camera::allCameras[0]->Min(t).x;
            ///////////////    //auto p_y = Camera::allCameras[0]->Max(t).y - ((cursorViewPortDistanceY / GamePanel::render_size.y) * cameraHeight);
            ///////////////#else
            ///////////////    float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().
            ///////////////      point_x;
            ///////////////    float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().
            ///////////////      point_y;
            ///////////////    auto p_x = ((cursorViewPortDistanceX / GetEnv().pWin->ClientRect().point_x) * cameraWidth) + Camera::allCameras[0]->
            ///////////////      Min(t).x;
            ///////////////    auto p_y = Camera::allCameras[0]->Max(t).y - ((cursorViewPortDistanceY / GetEnv().pWin->ClientRect().point_y) *
            ///////////////      cameraHeight);
            ///////////////#endif
            ///////////////  // std::cout << "p_x is " << p_x << std::endl;
            ///////////////  // std::cout << "p_y is " << p_y << std::endl;
            ///////////////  // std::cout << "------------------------------------" << std::endl;
            ///////////////  // 
            ///////////////  // std::cout << "cameraWidth is " << cameraWidth << std::endl;
            ///////////////  // std::cout << "cameraHeight is " << cameraHeight << std::endl;
            ///////////////  // std::cout << "------------------------------------" << std::endl;
            ///////////////  // 
            ///////////////  // std::cout << "InputManager::Instance().CurrentPosition().point_x is " << InputManager::Instance().CurrentPosition().point_x << std::endl;
            ///////////////  // std::cout << "InputManager::Instance().CurrentPosition().point_y is " << InputManager::Instance().CurrentPosition().point_y << std::endl;
            ///////////////  // std::cout << "------------------------------------" << std::endl;
            ///////////////  // 
            ///////////////  // std::cout << "GamePanel::render_pos.x is " << GamePanel::render_pos.x << std::endl;
            ///////////////  // std::cout << "GamePanel::render_pos.y is " << GamePanel::render_pos.y << std::endl;
            ///////////////  // std::cout << "------------------------------------" << std::endl;
            ///////////////  // 
            ///////////////  // std::cout << "cursorViewPortDistanceX is " << cursorViewPortDistanceX << std::endl;
            ///////////////  // std::cout << "cursorViewPortDistanceY is " << cursorViewPortDistanceY << std::endl;
            ///////////////  // std::cout << "------------------------------------" << std::endl;
            ///////////////
            ///////////////  //auto& t = em.GetComponent<Transform>({ 0 });
            ///////////////  //float cameraWidth = Camera::allCameras[0]->Max(t).x - Camera::allCameras[0]->Min(t).x;
            ///////////////  //float cameraHeight = Camera::allCameras[0]->Max(t).y - Camera::allCameras[0]->Min(t).y;
            ///////////////
            ///////////////  //float cameraWidth = Camera::editorCamera->Max(Camera::editorCameraTransform).x - Camera::editorCamera->Min(
            ///////////////  //    Camera::editorCameraTransform).x;
            ///////////////  //float cameraHeight = Camera::editorCamera->Max(Camera::editorCameraTransform).y - Camera::editorCamera->Min(
            ///////////////  //    Camera::editorCameraTransform).y;
            ///////////////  //float cursorViewPortDistanceX = GamePanel::mouse_pos.x - GamePanel::render_pos.x;
            ///////////////  //float cursorViewPortDistanceY = GamePanel::mouse_pos.y - GamePanel::render_pos.y;
            ///////////////  //curr_mouse.point_x = ((cursorViewPortDistanceX / GamePanel::render_pos.x) * cameraWidth) + Camera::editorCamera->Min(
            ///////////////  //    Camera::editorCameraTransform).x;
            ///////////////  //curr_mouse.point_y = Camera::editorCamera->Max(Camera::editorCameraTransform).y - ((cursorViewPortDistanceY /
            ///////////////  //    GamePanel::render_pos.y) * cameraHeight);
            ///////////////
            ///////////////  std::cout << "GamePanel::mouse_pos.x is " << GamePanel::mouse_pos.x << std::endl;
            ///////////////  std::cout << "GamePanel::mouse_pos.y is " << GamePanel::mouse_pos.y << std::endl;
            ///////////////  std::cout << "-----------------------------------------------" << std::endl;
            ///////////////
            ///////////////  //std::cout << "cursorViewPortDistanceX is " << cursorViewPortDistanceX << std::endl;
            ///////////////  //std::cout << "cursorViewPortDistanceY is " << cursorViewPortDistanceY << std::endl;
            ///////////////  //std::cout << "-----------------------------------------------" << std::endl;
            ///////////////
            ///////////////  std::cout << "curr_mouse.point_x is " << curr_mouse.point_x << std::endl;
            ///////////////  std::cout << "curr_mouse.point_y is " << curr_mouse.point_y << std::endl;
            ///////////////  std::cout << "-----------------------------------------------" << std::endl;
            ///////////////
            ///////////////  //std::cout << "p_x is " << p_x << std::endl;
            ///////////////  //std::cout << "p_y is " << p_y << std::endl;
            ///////////////  //std::cout << "-----------------------------------------------" << std::endl;
            ///////////////  
            ///////////////  //auto& player_pos = em.GetComponent<Transform>(UnitManager::GetPlayerID());
            ///////////////

            auto& t = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>({ 0 });
            float cameraWidth = Camera::allCameras[0]->Max(t).x - Camera::allCameras[0]->Min(t).x;
            float cameraHeight = Camera::allCameras[0]->Max(t).y - Camera::allCameras[0]->Min(t).y;

#ifdef DE_EDITOR
            float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GamePanel::render_pos.x;
            float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GamePanel::render_pos.y;
            auto p_x = ((cursorViewPortDistanceX / GamePanel::render_size.x) * cameraWidth) + Camera::allCameras[0]->Min(t).x;
            auto p_y = Camera::allCameras[0]->Max(t).y - ((cursorViewPortDistanceY / GamePanel::render_size.y) * cameraHeight);
#else
            float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().
                point_x;
            float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().
                point_y;
            auto p_x = ((cursorViewPortDistanceX / GetEnv().pWin->ClientRect().point_x) * cameraWidth) + Camera::allCameras[0]->
                Min(t).x;
            auto p_y = Camera::allCameras[0]->Max(t).y - ((cursorViewPortDistanceY / GetEnv().pWin->ClientRect().point_y) *
                cameraHeight);
#endif

            //std::cout << "curr_mouse is " << GamePanel::curr_mouse.point_x << ", " << GamePanel::curr_mouse.point_y << std::endl;
            //std::cout << "p_x is " << p_x << std::endl;
            //std::cout << "p_y is " << p_y << std::endl;
            //std::cout << "-----------------------------------------------" << std::endl;
            if (p_x > -0.74715)
                return true;

            return false;
        }
    }
  //void MouseCalculation::Update()
  //{
  //  ShootLeft();
  //  ShootRight();
  //  IsMouseOnRight();
  //  Profiler::Instance().Record("MouseCalculation System");
  //}
  //
  //void MouseCalculation::LateUpdate()
  //{
  //}
  

  
}
