#include "ViewportPanel.h"
#include "Input/InputManager.h"
#include "Render/Camera.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "Render/Camera.h"

namespace DeltaEngine
{

void Camera()
{
  ImGui::Begin( "Camera" );
  static float f = 0.0f;
  ImGui::Text( "Edit Camera Props" );                           // Display some text (you can use a format string too)
  ImGui::DragFloat3( "pos", (float *) &Camera::editorCamera->transform.position, 0.01f );
  ImGui::DragFloat( "size", (float *) &Camera::editorCamera->m_Size, 0.01f );
  ImGui::SliderFloat( "rot", &f, -180.0f, 180.0f, "%.1f", 1.0f );
  Camera::editorCamera->transform.rotation = Quaternion::AngleAxis( f, Vector3::forward() );
  ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
  ImGui::End();
}

ViewportPanel::ViewportPanel( std::string str ) :
  IPanel( str )
{
  m_enabled = true;
}

ViewportPanel::~ViewportPanel()
{
  m_enabled = false;
}

bool ViewportPanel::DraggedFileIn()
{
    if (InputManager::Get()->CurrentPosition().point_x >= GetTopLeft().x && InputManager::Get()->CurrentPosition().point_x <= GetBottomRight().x
        && InputManager::Get()->CurrentPosition().point_y >= GetTopLeft().y && InputManager::Get()->CurrentPosition().point_y <= GetBottomRight().y)
    {
        std::cout << "it is in Viewport panel!!!" << std::endl;
        return true;
    }
    return false;
}

void ViewportPanel::Render( bool isdragged )
{
  // camera properties
  Camera();

  ImGui::Begin( m_name.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove );
  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ImVec2 renderPos = ImGui::GetCursorScreenPos();     // gives top left of the window
  ImVec2 renderSize = ImGui::GetContentRegionAvail(); // gives height and width 
  float height = renderPos.y + renderSize.y;          // gets bottom right of the screen
  float width = renderPos.x + renderSize.x;           // gets bottom right of the screen
                                                      // check if cursor is in the viewport
  if ( InputManager::Get()->CurrentPosition().point_x >= renderPos.x && InputManager::Get()->CurrentPosition().point_x <= width
       && InputManager::Get()->CurrentPosition().point_y >= renderPos.y && InputManager::Get()->CurrentPosition().point_y <= height )
  {
    float cameraWidth = Camera::editorCamera->Max().x - Camera::editorCamera->Min().x;
    float cameraHeight = Camera::editorCamera->Max().y - Camera::editorCamera->Min().y;
    float cursorViewPortDistanceX = InputManager::Get()->CurrentPosition().point_x - renderPos.x;
    float cursorViewPortDistanceY = InputManager::Get()->CurrentPosition().point_y - renderPos.y;
    float newCursorX = ( cursorViewPortDistanceX / renderSize.x ) * cameraWidth + Camera::editorCamera->Min().x;
    float newCursorY = Camera::editorCamera->Max().y - ( cursorViewPortDistanceY / renderSize.y ) * cameraHeight;

    InputManager::Get()->SetCurrentCameraPosition( Point( (int) newCursorX, (int)newCursorY ) );
    //std::cout << "x is " << newCursorX << " and y is " << newCursorY << std::endl;
  }
  else
  {
    InputManager::Get()->SetCurrentCameraPosition( InputManager::Get()->CurrentPosition() );
    //std::cout << "x is " << InputManager::Get()->CurrentCameraPosition().point_x << " and y is " << InputManager::Get()->CurrentCameraPosition().point_y << std::endl;
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Camera::editorCamera->SetAspectRatio( viewportPanelSize.x, viewportPanelSize.y );
  Camera::editorCamera->SetViewportSize( viewportPanelSize.x );
  uint64_t textureID = Camera::editorCamera->GetFrameBuffer().GetColorAttachment();
  ImGui::Image( reinterpret_cast<void *>( textureID ), viewportPanelSize, ImVec2 { 0, 1 }, ImVec2 { 1, 0 } );
  ImGui::End();
}

ImVec2 ViewportPanel::GetTopLeft()
{
  return topLeft;
}

ImVec2 ViewportPanel::GetBottomRight()
{
  return bottomRight;
}
}
