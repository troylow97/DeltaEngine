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
#include "Components/RigidBody.h"
#include "Components/Collider.h"
#include "Components/Character.h"
#include "Input/InputManager.h"
#include "Physics/Collision.h"

//#include "DeltaEngine.h"

namespace DeltaEngine
{
    //DeltaEngineGlobalEnvironment env;

ImGuiLayer::ImGuiLayer()
  : Layer( "ImGuiLayer" )
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

void ImGuiLayer::OnEvent()
{

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

  bool selectingBoxEntity = false;
  bool selectingCircleEntity = false;

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
    if (InputManager::Get()->CurrentPosition().point_x >= renderPos.x && InputManager::Get()->CurrentPosition().point_x <= width
        && InputManager::Get()->CurrentPosition().point_y >= renderPos.y && InputManager::Get()->CurrentPosition().point_y <= height)
    {
        float cameraWidth = Camera::editorCamera->Max().x - Camera::editorCamera->Min().x;
        float cameraHeight = Camera::editorCamera->Max().y - Camera::editorCamera->Min().y;
        float cursorViewPortDistanceX = InputManager::Get()->CurrentPosition().point_x - renderPos.x;
        float cursorViewPortDistanceY = InputManager::Get()->CurrentPosition().point_y - renderPos.y;
        float newCursorX = (cursorViewPortDistanceX / renderSize.x) * cameraWidth + Camera::editorCamera->Min().x;
        float newCursorY = Camera::editorCamera->Max().y - (cursorViewPortDistanceY / renderSize.y) * cameraWidth;

        InputManager::Get()->SetCurrentCameraPosition(Point(newCursorX, newCursorY));

        //std::cout << "x is " << newCursorX << " and y is " << newCursorY << std::endl;
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
  // entities' properties
  {
      ImGui::Begin( "Entities" );
      ImGui::Text( "Edit Entities' Properties" );

	  static ImGuiComboFlags flags = 0;
	  const char* entityList[] = { "entity 0", "entity 1", "entity 2", "entity 3", "entity 4", "entity 5", "entity 6" };
	  static int entityIndex = 0;                         
	  const char* entityLabel = entityList[entityIndex];  // label for preview before selecting from combo

	  if (ImGui::BeginCombo("select entity", entityLabel, flags))
	  {
		  for (int n = 0; n < IM_ARRAYSIZE(entityList); n++)
		  {
			  const bool is_selected = (entityIndex == n);
			  if (ImGui::Selectable(entityList[n], is_selected))
			  {
				  entityIndex = n;
			  }
			  // set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			  if (is_selected)
			  {
				  ImGui::SetItemDefaultFocus();
			  }
		  }
		  ImGui::EndCombo();
	  }

	  ImGui::Text("");

      env.pECS->GetWorld().get_entity_manager().ForEach([&](EntityID& id1, Collider& c1, Transform& t1, RigidBody& r1)
      {  
         //rttr::type t = rttr::type::get_by_name( "DeltaEngine::Transform" ); 
         //std::string text = "ENTITY ";
         //text += std::to_string(id1.index);
         //ImGui::Text("");
         //ImGui::Text(text.c_str());
		 if (id1.index == entityIndex)
		 {
			 ImGui::Text("transform");
			 ImGui::DragFloat3("old pos", (float*)(&(t1.old_position)), 0.01f);
			 ImGui::DragFloat3("pos", (float*)(&(t1.position)), 0.01f);
			 ImGui::DragFloat3("size", (float*)(&(t1.scale)), 0.01f);
			 ImGui::DragFloat3("rot", (float*)(&(t1.rotation)), 0.01f);

			 ImGui::Text("rigidbody");
			 ImGui::DragFloat2("direction", (float*)(&(r1.Direction)), 0.01f);
			 ImGui::DragFloat2("velocity", (float*)(&(r1.Velocity)), 0.01f);
			 ImGui::DragFloat2("reflected vector", (float*)(&(r1.ReflectedVector)), 0.01f);
			 ImGui::DragFloat2("acceleration", (float*)(&(r1.Acceleration)), 0.01f);
			 ImGui::DragFloat("mass", (float*)(&(r1.Mass)), 0.01f);
			 ImGui::DragFloat("friction", (float*)(&(r1.Friction)), 0.01f);
			 ImGui::DragFloat("movespeed", (float*)(&(r1.Movespeed)), 0.01f);
			 ImGui::DragFloat("inherent acceleration", (float*)(&(r1.inherentAcceleration)), 0.01f);
			 ImGui::Checkbox("gravity", &(r1.hasGravity));
			 ImGui::Checkbox("moveable", &(r1.isMoveable));

			 ImGui::Text("collider");
			 ImGui::DragFloat2("inter point", (float*)(&(c1.interPoint)), 0.01f);
			 ImGui::DragFloat2("center", (float*)(&(c1.center)), 0.01f);
			 ImGui::DragFloat2("size", (float*)(&(c1.size)), 0.01f);
			 ImGui::Checkbox("collidable", &(c1.isCollideable));
		 }

      } );
      /* 
         Vector2 Center1 = col1.center;
         Vector2 Size1 = col1.size;
         Vector2 Center2 = col2.center;
         Vector2 Size2 = col2.size;
         ////////////////////////////////////////////////
         rttr tutorial
         https://www.rttr.org/doc/master/five_minute_tutorial_page.html
         
         type t = type::get_by_name("ns_3d::node");

         // will create an instance of ns_3d::node as std::shared_ptr<ns_3d::node>
         variant var = t.create({std::string("MyNode")});
         std::cout << var.get_type().get_name() << "\n";

         // sets/gets a property
         property prop = t.get_property("name");

         // remark: you can also set a member, although the instance is of type: 'std::shared_ptr<T>'
         prop.set_value(var, std::string("A New Name"));
         std::cout << prop.get_value(var).to_string() << "\n";

         // retrieve the stored meta data of the property
         std::cout << "MetaData TOOL_TIP: " << prop.get_metadata("TOOL_TIP").to_string() << "\n";

         // invoke a method
         method meth = t.get_method("set_visible");

         // remark: the 2nd argument will be provided automatically, because it has a default argument
         variant ret = meth.invoke(var, true);

         // a valid return value indicates a successful invoke
         std::cout << std::boolalpha << "invoke of method 'set_visible' was successfully: " << ret.is_valid() << "\n\n";

         // retrieve all properties
         std::cout << "'node' properties:" << "\n";
         for (auto& prop : t.get_properties())
         {
             std::cout << "  name: " << prop.get_name() << "\n";
             std::cout << "    type: " << prop.get_type().get_name() << "\n";
         }
         std::cout << "\n";

         // retrieve all methods
         std::cout << "'node' methods:" << "\n";
         for (auto& meth : t.get_methods())
         {
             std::cout << "  name: " << meth.get_name();
             std::cout << "  signature: " << meth.get_signature() << "\n";
             for (auto& info : meth.get_parameter_infos())
             {
                 std::cout << "    param " << info.get_index() << ": name: "<< info.get_name() << "\n";
             }
         }
         return 0;
      */

      ImGui::End();
  }
  // selecting entities to edit their properties
  {
	  if (InputManager::Get()->EntitySelected())
	  {
		  ImGui::Begin("Edit Entity's Properties");
		  ImGui::Text("");

		  env.pECS->GetWorld().get_entity_manager().ForEach([&](EntityID& id1, Collider& c1, Transform& t1, RigidBody& r1)
		  {
			  if (id1.index == InputManager::Get()->EntityIDSelected())
			  {
				  std::string text = "edit entity ";
				  text += std::to_string(id1.index);
				  text += "'s properties";
				  ImGui::Text(text.c_str());
				  ImGui::Text("");

				  ImGui::Text("transform");
				  ImGui::DragFloat3("old pos", (float*)(&(t1.old_position)), 0.01f);
				  ImGui::DragFloat3("pos", (float*)(&(t1.position)), 0.01f);
				  ImGui::DragFloat3("size", (float*)(&(t1.scale)), 0.01f);
				  ImGui::DragFloat3("rot", (float*)(&(t1.rotation)), 0.01f);

				  ImGui::Text("rigidbody");
				  ImGui::DragFloat2("direction", (float*)(&(r1.Direction)), 0.01f);
				  ImGui::DragFloat2("velocity", (float*)(&(r1.Velocity)), 0.01f);
				  ImGui::DragFloat2("reflected vector", (float*)(&(r1.ReflectedVector)), 0.01f);
				  ImGui::DragFloat2("acceleration", (float*)(&(r1.Acceleration)), 0.01f);
				  ImGui::DragFloat("mass", (float*)(&(r1.Mass)), 0.01f);
				  ImGui::DragFloat("friction", (float*)(&(r1.Friction)), 0.01f);
				  ImGui::DragFloat("movespeed", (float*)(&(r1.Movespeed)), 0.01f);
				  ImGui::DragFloat("inherent acceleration", (float*)(&(r1.inherentAcceleration)), 0.01f);
				  ImGui::Checkbox("gravity", &(r1.hasGravity));
				  ImGui::Checkbox("moveable", &(r1.isMoveable));

				  ImGui::Text("collider");
				  ImGui::DragFloat2("inter point", (float*)(&(c1.interPoint)), 0.01f);
				  ImGui::DragFloat2("center", (float*)(&(c1.center)), 0.01f);
				  ImGui::DragFloat2("size", (float*)(&(c1.size)), 0.01f);
				  ImGui::Checkbox("collidable", &(c1.isCollideable));
			  }

		  });
		  ImGui::End();
	  }
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
