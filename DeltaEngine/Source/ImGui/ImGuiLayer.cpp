#include "ImGuiLayer.h"
#include "Render/Window.h"
#include "Render/OpenGLSystem.h"
#include "Render/TextRenderer.h"
#include "Render/ParticleSystem.h"
#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_opengl3.h>
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

#include "Input/InputManager.h"

#include "Core/Utils/FileDialog.h"
//#include "DeltaEngine.h"

namespace DeltaEngine
{
    //DeltaEngineGlobalEnvironment env;

ImGuiLayer::ImGuiLayer()
  : Layer( "ImGuiLayer" )/*,
    wp {"World"}*/
{}

void ImGuiLayer::OnAttach()
{
  // ----------------
  // ImGui setup
  // -----------------

  // Application init: create a dear imgui context, setup some options, load fonts
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  // TODO: Set optional io.ConfigFlags values, e.g. 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard' to enable keyboard controls.
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
  //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
  //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
  // TODO: Fill optional fields of the io structure later.
  // TODO: Load TTF/OTF fonts if you don't want to use the default font.
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Initialize helper Platform and Renderer bindings (here we are using imgui_impl_win32.cpp and imgui_impl_dx11.cpp)
  ImGui_ImplWin32_Init( env.pWin->GetHandle(), RenderModule::openGLSystem->GetGLContext() );
  ImGui_ImplOpenGL3_Init( "#version 410" );
  // ----------------
  // ImGui setup end
  // -----------------
}

void ImGuiLayer::OnDetach()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event* e)
{
    //wp.DraggedFileIn();
}

void ImGuiLayer::Begin()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  // Note: Switch this to true to enable dockspace
  static bool dockspaceOpen = true;
  static bool opt_fullscreen_persistant = true;
  bool opt_fullscreen = opt_fullscreen_persistant;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  auto &em = env.pECS->GetWorld().get_entity_manager();
  auto &ref = ImGui::GetIO();
  std::memcpy( &ref.KeysDown[0], InputManager::Get()->GetKeys(), 256 );

  //if ( ImGui::IsKeyDown( DEVK_LCTRL ) && ImGui::IsKeyReleased( DEVK_N ) )
  //  GetEnv().pECS->GetWorld().get_entity_manager().Clear();

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
  // because it would be confusing to have two docking targets within each others.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if ( opt_fullscreen )
  {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos( viewport->Pos );
    ImGui::SetNextWindowSize( viewport->Size );
    ImGui::SetNextWindowViewport( viewport->ID );
    ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
    ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
  if ( dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode )
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
  // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
  // all active windows docked into it will lose their parent and become undocked.
  // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
  // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
  ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
  ImGui::Begin( "DockSpace Demo", &dockspaceOpen, window_flags );
  ImGui::PopStyleVar();

  if ( opt_fullscreen )
    ImGui::PopStyleVar( 2 );

  // DockSpace
  ImGuiIO &io = ImGui::GetIO();
  if ( io.ConfigFlags & ImGuiConfigFlags_DockingEnable )
  {
    ImGuiID dockspace_id = ImGui::GetID( "MyDockSpace" );
    ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), dockspace_flags );
  }

  if ( ImGui::BeginMenuBar() )
  {
    if ( ImGui::BeginMenu( "File" ) )
    {
      // Disabling fullscreen would allow the window to be moved to the front of other windows, 
      // which we can't undo at the moment without finer window depth/z control.
      //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

      if ( ImGui::MenuItem( "Print" ) )
        std::cout << "This is a test print" << std::endl;
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }
  // camera properties
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
  // viewport
  {
    ImGui::Begin( "Viewport" );

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

      InputManager::Get()->SetCurrentCameraPosition( Point( newCursorX, newCursorY ) );
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
  // renderers
  if ( RenderModule::allRenderers.size() > 0 )
  {
    ImGui::Begin( "SpriteRenderer1" );

    static float f = 0.0f;
    ImGui::Text( "Edit Background Props" );                           // Display some text (you can use a format string too)
    ImGui::Checkbox( "Active", &RenderModule::allRenderers[0]->m_Active );
    ImGui::DragFloat3( "pos", (float *) &RenderModule::allRenderers[0]->transform.position, 0.01f );
    ImGui::DragFloat3( "size", (float *) &RenderModule::allRenderers[0]->transform.scale, 0.01f );
    ImGui::SliderFloat( "rot", &f, -180.0f, 180.0f, "%.1f", 1.0f );
    RenderModule::allRenderers[0]->transform.rotation = Quaternion::AngleAxis( f, Vector3::forward() );
    ImGui::Text( "Sprite Name: %s", dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[0] )->sprite.GetName().c_str() );
    ImGui::Checkbox( "Flip X", &dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[0] )->m_FlipX );
    ImGui::Checkbox( "Flip Y", &dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[0] )->m_FlipY );
    ImGui::Checkbox( "Shaded", &dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[0] )->m_Shaded );
    ImGui::Checkbox( "Wireframe", &dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[0] )->m_Wireframe );

    ImGui::End();
  }
  if ( RenderModule::allRenderers.size() > 1 )
  {
    ImGui::Begin( "SpriteRenderer2" );

    static float f = 0.0f;
    ImGui::Text( "Edit Sprite Props" );                           // Display some text (you can use a format string too)
    ImGui::Checkbox( "Active", &RenderModule::allRenderers[1]->m_Active );
    ImGui::DragFloat3( "pos", (float *) &RenderModule::allRenderers[1]->transform.position, 0.01f );
    ImGui::DragFloat3( "size", (float *) &RenderModule::allRenderers[1]->transform.scale, 0.01f );
    ImGui::SliderFloat( "rot", &f, -180.0f, 180.0f, "%.1f", 1.0f );
    RenderModule::allRenderers[1]->transform.rotation = Quaternion::AngleAxis( f, Vector3::forward() );
    ImGui::Text( "Sprite Name: %s", dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[1] )->sprite.GetName().c_str() );
    ImGui::Checkbox( "Flip X", &dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[1] )->m_FlipX );
    ImGui::Checkbox( "Flip Y", &dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[1] )->m_FlipY );
    ImGui::Checkbox( "Shaded", &dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[1] )->m_Shaded );
    ImGui::Checkbox( "Wireframe", &dynamic_cast<SpriteRenderer *>( RenderModule::allRenderers[1] )->m_Wireframe );

    ImGui::End();
  }
  if ( RenderModule::allRenderers.size() > 2 )
  {
    ImGui::Begin( "TextRenderer" );

    static float f = 0.0f;
    ImGui::Text( "Edit Text Props" );                           // Display some text (you can use a format string too)
    ImGui::Checkbox( "Active", &RenderModule::allRenderers[2]->m_Active );
    ImGui::DragFloat3( "pos", (float *) &RenderModule::allRenderers[2]->transform.position, 0.01f );
    ImGui::DragFloat3( "size", (float *) &RenderModule::allRenderers[2]->transform.scale, 0.01f );
    ImGui::SliderFloat( "rot", &f, -180.0f, 180.0f, "%.1f", 1.0f );
    RenderModule::allRenderers[2]->transform.rotation = Quaternion::AngleAxis( f, Vector3::forward() );

    ImGui::End();
  }
  if ( RenderModule::allRenderers.size() > 3 )
  {
    ImGui::Begin( "Particle System" );

    static float f = 0.0f;
    ImGui::Checkbox( "Active", &RenderModule::allRenderers[3]->m_Active );
    ImGui::Text( "Edit Particle System Props" );                           // Display some text (you can use a format string too)
    ImGui::DragFloat3( "pos", (float *) &RenderModule::allRenderers[3]->transform.position, 0.01f );
    ImGui::DragFloat3( "size", (float *) &RenderModule::allRenderers[3]->transform.scale, 0.01f );
    ImGui::SliderFloat( "rot", &f, -180.0f, 180.0f, "%.1f", 1.0f );
    RenderModule::allRenderers[3]->transform.rotation = Quaternion::AngleAxis( f, Vector3::forward() );

    ImGui::End();
  }
  // selection

  //{
  //  if ( ImGui::BeginMainMenuBar() )
  //  {
  //    if ( ImGui::BeginMenu( "main" ) )
  //    {
  //      if ( ImGui::MenuItem( "new scene" ) )
  //      {
  //          /* Do stuff */
  //      }
  //      if ( ImGui::MenuItem( "load scene" ) )
  //      {
  //          /* Do stuff */
  //      }
  //      if ( ImGui::MenuItem( "save scene" ) )
  //      {
  //          /* Do stuff */
  //      }
  //      if ( ImGui::MenuItem( "quit" ) )
  //      {
  //          /* Do stuff */
  //      }
  //
  //      ImGui::EndMenu();
  //    }
  //    if ( ImGui::BeginMenu( "entity" ) )
  //    {
  //      if ( ImGui::MenuItem( "add entity" ) )
  //      {
  //          /* Do stuff */
  //      }
  //      if ( ImGui::MenuItem( "clone entity" ) )
  //      {
  //          /* Do stuff */
  //      }
  //      if ( ImGui::MenuItem( "save entity" ) )
  //      {
  //          /* Do stuff */
  //      }
  //      if ( ImGui::MenuItem( "delete entity" ) )
  //      {
  //          /* Do stuff */
  //      }
  //
  //      ImGui::EndMenu();
  //    }
  //    if ( ImGui::BeginMenu( "view" ) )
  //    {
  //      if ( ImGui::MenuItem( "hierarchy" ) )
  //      {
  //          /* Do stuff */
  //      }
  //      if ( ImGui::MenuItem( "world" ) )
  //      {
  //          /* Do stuff */
  //      }
  //
  //      ImGui::EndMenu();
  //    }
  //
  //    ImGui::EndMainMenuBar();
  //  }
  //}
  // world
  
  //wp.Render(false);

  //wp.DraggedFileIn();
  ////////////////////////////{
  ////////////////////////////  ImGui::Begin( "World" );
  ////////////////////////////
  ////////////////////////////  if ( ImGui::TreeNode( "Entities" ) )
  ////////////////////////////  {
  ////////////////////////////
  ////////////////////////////    for ( size_t e_id = 0; e_id < em.GetEntities().size(); e_id++ )
  ////////////////////////////    {
  ////////////////////////////      static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_None;
  ////////////////////////////
  ////////////////////////////      ImGuiTreeNodeFlags node_flags = base_flags;
  ////////////////////////////      node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
  ////////////////////////////
  ////////////////////////////      ImGui::TreeNodeEx( (void *) (intptr_t) e_id, node_flags, "entity %d", e_id );
  ////////////////////////////      if ( ImGui::IsItemClicked() )
  ////////////////////////////      {
  ////////////////////////////        InputManager::Get()->SetEntitySelected( true );
  ////////////////////////////        InputManager::Get()->SetEntityIDSelected( e_id );
  ////////////////////////////      }
  ////////////////////////////    }
  ////////////////////////////    ImGui::TreePop();
  ////////////////////////////  }
  ////////////////////////////  if ( ImGui::TreeNode( "Environment" ) )
  ////////////////////////////  {
  ////////////////////////////    if ( ImGui::TreeNode( "NOTHING HERE" ) )
  ////////////////////////////    {
  ////////////////////////////      ImGui::Text( "i told you there's nothing already lol" );
  ////////////////////////////      ImGui::TextColored( ImVec4( 1.0f, 0.0f, 1.0f, 1.0f ), "pink" );
  ////////////////////////////      ImGui::TextColored( ImVec4( 1.0f, 1.0f, 0.0f, 1.0f ), "yellow" );
  ////////////////////////////      ImGui::TextColored( ImVec4( 0.25f, 0.875f, 0.8125f, 1.0f ), "clara's fav color is turquoise" );
  ////////////////////////////      ImGui::TreePop();
  ////////////////////////////    }
  ////////////////////////////    ImGui::TreePop();
  ////////////////////////////  }
  ////////////////////////////  if ( ImGui::TreeNode( "Segments" ) )
  ////////////////////////////  {
  ////////////////////////////    for ( int i = 0; i < 6; i++ )
  ////////////////////////////    {
  ////////////////////////////      static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_None;
  ////////////////////////////
  ////////////////////////////      ImGuiTreeNodeFlags node_flags = base_flags;
  ////////////////////////////      node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
  ////////////////////////////
  ////////////////////////////      ImGui::TreeNodeEx( (void *) (intptr_t) i, node_flags, "fake one sike %d", i );
  ////////////////////////////    }
  ////////////////////////////    ImGui::TreePop();
  ////////////////////////////  }
  ////////////////////////////     //Vector2 Center1 = col1.center;
  ////////////////////////////     //Vector2 Size1 = col1.size;
  ////////////////////////////     //Vector2 Center2 = col2.center;
  ////////////////////////////     //Vector2 Size2 = col2.size;
  ////////////////////////////     ////////////////////////////////////////////////
  ////////////////////////////     //rttr tutorial
  ////////////////////////////     //https://www.rttr.org/doc/master/five_minute_tutorial_page.html
  ////////////////////////////     //
  ////////////////////////////     //type t = type::get_by_name("ns_3d::node");
  ////////////////////////////     //
  ////////////////////////////     //// will create an instance of ns_3d::node as std::shared_ptr<ns_3d::node>
  ////////////////////////////     //variant var = t.create({std::string("MyNode")});
  ////////////////////////////     //std::cout << var.get_type().get_name() << "\n";
  ////////////////////////////     //
  ////////////////////////////     //// sets/gets a property
  ////////////////////////////     //property prop = t.get_property("name");
  ////////////////////////////     //
  ////////////////////////////     //// remark: you can also set a member, although the instance is of type: 'std::shared_ptr<T>'
  ////////////////////////////     //prop.set_value(var, std::string("A New Name"));
  ////////////////////////////     //std::cout << prop.get_value(var).to_string() << "\n";
  ////////////////////////////     //
  ////////////////////////////     //// retrieve the stored meta data of the property
  ////////////////////////////     //std::cout << "MetaData TOOL_TIP: " << prop.get_metadata("TOOL_TIP").to_string() << "\n";
  ////////////////////////////     //
  ////////////////////////////     //// invoke a method
  ////////////////////////////     //method meth = t.get_method("set_visible");
  ////////////////////////////     //
  ////////////////////////////     //// remark: the 2nd argument will be provided automatically, because it has a default argument
  ////////////////////////////     //variant ret = meth.invoke(var, true);
  ////////////////////////////     //
  ////////////////////////////     //// a valid return value indicates a successful invoke
  ////////////////////////////     //std::cout << std::boolalpha << "invoke of method 'set_visible' was successfully: " << ret.is_valid() << "\n\n";
  ////////////////////////////     //
  ////////////////////////////     //// retrieve all properties
  ////////////////////////////     //std::cout << "'node' properties:" << "\n";
  ////////////////////////////     //for (auto& prop : t.get_properties())
  ////////////////////////////     //{
  ////////////////////////////     //    std::cout << "  name: " << prop.get_name() << "\n";
  ////////////////////////////     //    std::cout << "    type: " << prop.get_type().get_name() << "\n";
  ////////////////////////////     //}
  ////////////////////////////     //std::cout << "\n";
  ////////////////////////////     //
  ////////////////////////////     //// retrieve all methods
  ////////////////////////////     //std::cout << "'node' methods:" << "\n";
  ////////////////////////////     //for (auto& meth : t.get_methods())
  ////////////////////////////     //{
  ////////////////////////////     //    std::cout << "  name: " << meth.get_name();
  ////////////////////////////     //    std::cout << "  signature: " << meth.get_signature() << "\n";
  ////////////////////////////     //    for (auto& info : meth.get_parameter_infos())
  ////////////////////////////     //    {
  ////////////////////////////     //        std::cout << "    param " << info.get_index() << ": name: "<< info.get_name() << "\n";
  ////////////////////////////     //    }
  ////////////////////////////     //}
  ////////////////////////////     //return 0;
  ////////////////////////////
  ////////////////////////////  ImGui::End();
  ////////////////////////////}
  // selecting entities to edit their properties
    
 // inspector

 //if ( InputManager::Get()->EntitySelected() )
 //{
 //  ImGui::Begin( "Properties Inspector" );
 //
 //  size_t index = InputManager::Get()->EntityIDSelected();
 //
 //  std::string text = "Edit Entity ";
 //  text += std::to_string( index );
 //  text += "'s Properties";
 //  ImGui::Text( text.c_str() );
 //  ImGui::Text( "" );
 //
 //  static char str1[128] = "";
 //  ImGui::SetNextItemWidth( 100 );
 //  ImGui::InputTextWithHint( "texture", "texture name", str1, IM_ARRAYSIZE( str1 ) );
 //  ImGui::SameLine();
 //  static int clicked = 0;
 //  ImGui::PushStyleColor( ImGuiCol_Button, ( ImVec4( 0.0f, 0.775f, 0.4125f, 1.0f ) ) );
 //  ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ( ImVec4( 0.0f, 0.825f, 0.4125f, 1.0f ) ) );
 //  ImGui::PushStyleColor( ImGuiCol_ButtonActive, ( ImVec4( 0.0f, 0.875f, 0.4125f, 1.0f ) ) );
 //  if ( ImGui::Button( "Browse" ) )
 //  {
 //    clicked++;
 //  }
 //  ImGui::PopStyleColor( 3 );
 //  if ( clicked & 1 )
 //  {
 //    ImGui::Begin( "Sprite Selection" );
 //    {
 //      ImGui::Text( "Character" );
 //
 //      std::vector<Sprite> spritelist;
 //      uint64_t textureID;
 //      Sprite running = { "run", 5 };
 //      Sprite idling = { "idle", 0 };
 //
 //      spritelist.push_back( running );
 //      spritelist.push_back( idling );
 //
 //      for ( int i = 0; i < spritelist.size(); ++i )
 //      {
 //        textureID = spritelist[i].GetTexture()->GetRendererID();
 //
 //        if ( ImGui::ImageButton( reinterpret_cast<void *>( textureID ),
 //             ImVec2 { 32,32 },
 //             ImVec2 { spritelist[i].GetOffset().x, spritelist[i].GetOffset().y },
 //             ImVec2 { spritelist[i].GetOffset().x + spritelist[i].GetTiling().x, spritelist[i].GetOffset().y + spritelist[i].GetTiling().y } ) )
 //        {
 //          static int textureClicked = 0;
 //          textureClicked++;
 //          std::cout << "textureclicked is " << textureClicked << std::endl;
 //
 //          if ( textureClicked & 1 )
 //          {
 //            strcpy( str1, spritelist[i].GetName().c_str() );
 //          }
 //        }
 //        ImGui::SameLine();
 //      }
 //    }
 //    ImGui::Text( "" );
 //    ImGui::Text( "" );
 //    {
 //        // to show examples for more only
 //      ImGui::Text( "Background" );
 //
 //      Sprite bg = { "bg", 0 };
 //      uint64_t textureID = bg.GetTexture()->GetRendererID();
 //
 //      if ( ImGui::ImageButton( reinterpret_cast<void *>( textureID ),
 //           ImVec2 { 32,32 },
 //           ImVec2 { bg.GetOffset().x, bg.GetOffset().y },
 //           ImVec2 { bg.GetOffset().x + bg.GetTiling().x, bg.GetOffset().y + bg.GetTiling().y } ) )
 //      {
 //        static int textureClicked = 0;
 //        textureClicked++;
 //
 //        if ( textureClicked & 1 )
 //        {
 //          strcpy( str1, bg.GetName().c_str() );
 //        }
 //      }
 //      ImGui::SameLine();
 //    }
 //    ImGui::Text( "" );
 //    ImGui::Text( "" );
 //
 //    ImGui::End();
 //  }
 //  ImGui::Text( "" );
 //
 //  for ( auto &ref : em.GetEntityArchetype( InputManager::Get()->EntityIDSelected() ) )
 //  {
 //    rttr::instance &instance = em.GetComponent( { InputManager::Get()->EntityIDSelected() }, ref.meta->bits );
 //
 //    ImGui::Text( instance.get_type().get_name().to_string().c_str() );
 //    auto properties = instance.get_type().get_properties();
 //    for ( auto property : properties )
 //    {
 //      rttr::variant value = property.get_value( instance );
 //      if ( !value )
 //        continue;
 //
 //      if ( property.get_type().get_name() == "float*" )
 //        ImGui::DragFloat( property.get_name().to_string().c_str(), ( value.get_value<float *>() ), 0.01f );
 //      else if ( property.get_type().get_name() == "vector2*" )
 //        ImGui::DragFloat2( property.get_name().to_string().c_str(), (float *) ( value.get_value<Vector2 *>() ), 0.01f );
 //      else if ( property.get_type().get_name() == "vector3*" )
 //        ImGui::DragFloat3( property.get_name().to_string().c_str(), (float *) ( value.get_value<Vector3 *>() ), 0.01f );
 //      else if ( property.get_type().get_name() == "bool*" )
 //        ImGui::Checkbox( property.get_name().to_string().c_str(), ( value.get_value<bool *>() ) );
 //    }
 //  }
 //
 //  ImGui::End();
 //}

// selection panel 
  {
    //ImGui::Begin( "Panel Selection" );

      //DropManager dm;
      //if (dm.IsInPanel())
      //{
      //    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern))	// we use an external source (i.e. not ImGui-created)
      //    {
      //        // replace "FILES" with whatever identifier you want - possibly dependant upon what type of files are being dragged
      //        // you can specify a payload here with parameter 2 and the sizeof(parameter) for parameter 3.
      //        // I store the payload within a vector of strings within the application itself so don't need it.
      //        ImGui::SetDragDropPayload("FILES", nullptr, 0);
      //        ImGui::BeginTooltip();
      //        ImGui::Text("FILES");
      //        ImGui::EndTooltip();
      //        ImGui::EndDragDropSource();
      //    }
      //}
      //if (ImGui::BeginDragDropTarget())
      //{
      //    if (ImGui::AcceptDragDropPayload("FILES"))  // or: const ImGuiPayload* payload = ... if you sent a payload in the block above
      //    {
      //        std::vector<std::string> draggedFiles;
      //        // draggedFiles is my vector of strings, how you handle your payload is up to you
      //        for (const auto& file : draggedFiles)
      //        {
      //            // do something with file
      //            std::cout << "files are accepted!!" << std::endl;
      //        }
      //    }
      //
      //    ImGui::EndDragDropTarget();
      //}
      //if (ImGui::TreeNode("Images"))
      //{
      //    //ImGuiIO& io = ImGui::GetIO();
      //    //
      //    //// Below we are displaying the font texture because it is the only texture we have access to inside the demo!
      //    //// Remember that ImTextureID is just storage for whatever you want it to be. It is essentially a value that
      //    //// will be passed to the rendering backend via the ImDrawCmd structure.
      //    //// If you use one of the default imgui_impl_XXXX.cpp rendering backend, they all have comments at the top
      //    //// of their respective source file to specify what they expect to be stored in ImTextureID, for example:
      //    //// - The imgui_impl_dx11.cpp renderer expect a 'ID3D11ShaderResourceView*' pointer
      //    //// - The imgui_impl_opengl3.cpp renderer expect a GLuint OpenGL texture identifier, etc.
      //    //// More:
      //    //// - If you decided that ImTextureID = MyEngineTexture*, then you can pass your MyEngineTexture* pointers
      //    ////   to ImGui::Image(), and gather width/height through your own functions, etc.
      //    //// - You can use ShowMetricsWindow() to inspect the draw data that are being passed to your renderer,
      //    ////   it will help you debug issues if you are confused about it.
      //    //// - Consider using the lower-level ImDrawList::AddImage() API, via ImGui::GetWindowDrawList()->AddImage().
      //    //// - Read https://github.com/ocornut/imgui/blob/master/docs/FAQ.md
      //    //// - Read https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
      //    //ImTextureID my_tex_id = io.Fonts->TexID;
      //    //float my_tex_w = (float)io.Fonts->TexWidth;
      //    //float my_tex_h = (float)io.Fonts->TexHeight;
      //    //{
      //    //    ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
      //    //    ImVec2 pos = ImGui::GetCursorScreenPos();
      //    //    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
      //    //    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
      //    //    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
      //    //    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
      //    //    ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
      //    //    if (ImGui::IsItemHovered())
      //    //    {
      //    //        ImGui::BeginTooltip();
      //    //        float region_sz = 32.0f;
      //    //        float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
      //    //        float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
      //    //        float zoom = 4.0f;
      //    //        if (region_x < 0.0f) { region_x = 0.0f; }
      //    //        else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
      //    //        if (region_y < 0.0f) { region_y = 0.0f; }
      //    //        else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
      //    //        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
      //    //        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
      //    //        ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
      //    //        ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
      //    //        ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
      //    //        ImGui::EndTooltip();
      //    //    }
      //    //}
      //    ImGui::TreePop();
      //}
    //ImGui::End();
  }


  ImGui::End();
}

void ImGuiLayer::End()
{
  //ImGuiIO& io = ImGui::GetIO();
  //Application& app = Application::Get();
  //io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

  // Update and Render additional Platform Windows
  // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
  //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
  if ( ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
  {
    HDC backup_current_context = RenderModule::openGLSystem->GetWindowContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    wglMakeCurrent( backup_current_context, RenderModule::openGLSystem->GetGLContext() );
  }
  // ----------------
  // ImGui render end
  // -----------------
}
}
