#include "ImGui/Panels/MainMenuBar.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "Core/Utils/FileDialog.h"
#include "Input/InputManager.h"
#include "Input/Keys.h"

namespace DeltaEngine
{

void OpenFile()
{
  std::optional<std::string> path = FileDialogs::OpenFile( "DeltaEngine Scene (*.json)\0*.json\0" );

  if ( path )
    GetEnv().pECS->GetWorld().Load( *path );
}

void SaveFile()
{
  std::optional<std::string> path = FileDialogs::SaveFile( "DeltaEngine Scene (*.json)\0*\0" );

  if ( path )
    GetEnv().pECS->GetWorld().Save( *path );
}

MainMenuBar::MainMenuBar( std::string str ) :
  IPanel( str )
{

}

MainMenuBar::~MainMenuBar()
{
  m_enabled = false;
}

bool MainMenuBar::Render( bool )
{
    //ImGui::Begin(m_name.c_str());

  topLeft = ImGui::GetWindowContentRegionMin();
  bottomRight = ImGui::GetWindowContentRegionMax();

  topLeft.x += ImGui::GetWindowPos().x;
  topLeft.y += ImGui::GetWindowPos().y;
  bottomRight.x += ImGui::GetWindowPos().x;
  bottomRight.y += ImGui::GetWindowPos().y;

  if ( ImGui::IsKeyDown( DEVK_LCTRL ) && ImGui::IsKeyReleased( DEVK_O ) )
    OpenFile();
  if ( ImGui::IsKeyDown( DEVK_LCTRL ) && ImGui::IsKeyReleased( DEVK_S ) )
    SaveFile();
  if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyDown(DEVK_LSHIFT) && ImGui::IsKeyReleased( DEVK_A) )
    GetEnv().pECS->GetWorld().get_entity_manager().CreateEntity();
  if ( InputManager::Get()->EntitySelected() && ImGui::IsKeyReleased(DEVK_DELETE))
  {
    GetEnv().pECS->GetWorld().get_entity_manager().DestroyEntity( { InputManager::Get()->EntityIDSelected() } );
    InputManager::Get()->SetEntitySelected( false );
    InputManager::Get()->SetEntityIDSelected( 0 );
    DeltaEngine_CORE_TRACE( "DELETED ENTITIES" );
  }


  if ( ImGui::BeginMainMenuBar() )
  {
    if ( ImGui::BeginMenu( "Scene" ) )
    {
      if ( ImGui::MenuItem( "New", " Ctrl+N" ) )
      {
        GetEnv().pECS->GetWorld().get_entity_manager().Clear();
      }
      if ( ImGui::MenuItem( "Open", " Ctrl+O" ) )
      {
        OpenFile();
      }
      if ( ImGui::MenuItem( "Save", " Ctrl+S" ) )
      {
        SaveFile();
      }
      //if (ImGui::MenuItem("quit"))
      //{
      //    /* Do stuff */
      //}

      ImGui::EndMenu();
    }
    if ( ImGui::BeginMenu( "Entities" ) )
    {
      if ( ImGui::MenuItem( "Add Entity", " Ctrl+Shift+A" ) )
      {
        GetEnv().pECS->GetWorld().get_entity_manager().CreateEntity();
      }
      if ( ImGui::MenuItem( "clone entity" ) )
      {
        
      }
      if ( ImGui::MenuItem( "Delete Entity", " Del" ) )
      {
        GetEnv().pECS->GetWorld().get_entity_manager().DestroyEntity( { InputManager::Get()->EntityIDSelected() } );
      }

      ImGui::EndMenu();
    }
    if ( ImGui::BeginMenu( "view" ) )
    {
      if ( ImGui::MenuItem( "hierarchy" ) )
      {
          /* Do stuff */
      }
      if ( ImGui::MenuItem( "world" ) )
      {
          /* Do stuff */
      }

      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  return m_enabled;
}

//bool MainMenuBar::DraggedFileIn()
//{
//    if (InputManager::Get()->CurrentPosition().point_x >= topLeft.x && InputManager::Get()->CurrentPosition().point_x <= bottomRight.x
//        && InputManager::Get()->CurrentPosition().point_y >= topLeft.y && InputManager::Get()->CurrentPosition().point_y <= bottomRight.y)
//    {
//        std::cout << "it is in mainmenubar panel!!!" << std::endl;
//        return true;
//    }
//    return false;
//}

ImVec2 MainMenuBar::GetTopLeft()
{
  return topLeft;
}

ImVec2 MainMenuBar::GetBottomRight()
{
  return bottomRight;
}
}
