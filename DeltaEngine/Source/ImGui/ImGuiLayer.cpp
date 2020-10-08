#include "ImGuiLayer.h"
#include "Render/Window.h"
#include "Render/OpenGLSystem.h"
#include "Render/TextRenderer.h"
#include "Render/ParticleSystem.h"
#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_opengl3.h>
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnAttach()
	{
		// ----------------
		// ImGui setup
		// -----------------

		// Application init: create a dear imgui context, setup some options, load fonts
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
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
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Initialize helper Platform and Renderer bindings (here we are using imgui_impl_win32.cpp and imgui_impl_dx11.cpp)
		ImGui_ImplWin32_Init(env.pWin->GetHandle(), RenderModule::openGLSystem->GetGLContext());
		ImGui_ImplOpenGL3_Init("#version 410");
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

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Print"))
					std::cout << "This is a test print" << std::endl;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		// camera properties
		{
			ImGui::Begin("Camera");
			static float f = 0.0f;
			ImGui::Text("Edit Camera Props");                           // Display some text (you can use a format string too)
			ImGui::DragFloat3("pos", (float*)&Camera::editorCamera->transform.position, 0.01f);
			ImGui::DragFloat("size", (float*)&Camera::editorCamera->m_Size, 0.01f);
			ImGui::SliderFloat("rot", &f, -180.0f, 180.0f, "%.1f", 1.0f);
			Camera::editorCamera->transform.rotation = Quaternion::AngleAxis(f, Vector3::forward());
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		// viewport
		{
			ImGui::Begin("Viewport");

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			Camera::editorCamera->SetAspectRatio(viewportPanelSize.x, viewportPanelSize.y);
			Camera::editorCamera->SetViewportSize(viewportPanelSize.x);
			uint64_t textureID = Camera::editorCamera->GetFrameBuffer().GetColorAttachment();
			ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
		}
		// renderers
		if (RenderModule::allRenderers.size() > 0)
		{
			ImGui::Begin("SpriteRenderer");

			static float f = 0.0f;
			ImGui::Text("Edit Sprite Props");                           // Display some text (you can use a format string too)
			ImGui::Checkbox("Active", &RenderModule::allRenderers[0]->m_Active);
			ImGui::DragFloat3("pos", (float*)&RenderModule::allRenderers[0]->transform.position, 0.01f);
			ImGui::DragFloat3("size", (float*)&RenderModule::allRenderers[0]->transform.scale, 0.01f);
			ImGui::SliderFloat("rot", &f, -180.0f, 180.0f, "%.1f", 1.0f);
			RenderModule::allRenderers[0]->transform.rotation = Quaternion::AngleAxis(f, Vector3::forward());
			ImGui::Text("Sprite Name: %s", dynamic_cast<SpriteRenderer*>(RenderModule::allRenderers[0])->sprite.GetName().c_str());
			ImGui::Checkbox("Flip X", &dynamic_cast<SpriteRenderer*>(RenderModule::allRenderers[0])->m_FlipX);
			ImGui::Checkbox("Flip Y", &dynamic_cast<SpriteRenderer*>(RenderModule::allRenderers[0])->m_FlipY);
			ImGui::Checkbox("Shaded", &dynamic_cast<SpriteRenderer*>(RenderModule::allRenderers[0])->m_Shaded);
			ImGui::Checkbox("Wireframe", &dynamic_cast<SpriteRenderer*>(RenderModule::allRenderers[0])->m_Wireframe);

			ImGui::End();
		}
		if (RenderModule::allRenderers.size() > 1)
		{
			ImGui::Begin("TextRenderer");

			static float f = 0.0f;
			static char str[128];
			ImGui::Text("Edit Text Props");                           // Display some text (you can use a format string too)
			ImGui::Checkbox("Active", &RenderModule::allRenderers[1]->m_Active);
			ImGui::DragFloat3("pos", (float*)&RenderModule::allRenderers[1]->transform.position, 0.01f);
			ImGui::DragFloat3("size", (float*)&RenderModule::allRenderers[1]->transform.scale, 0.01f);
			ImGui::SliderFloat("rot", &f, -180.0f, 180.0f, "%.1f", 1.0f);
			ImGui::InputText("display text", str, 128);
			RenderModule::allRenderers[1]->transform.rotation = Quaternion::AngleAxis(f, Vector3::forward());
			dynamic_cast<TextRenderer*>(RenderModule::allRenderers[1])->text = str;

			ImGui::End();
		}
		if (RenderModule::allRenderers.size() > 2)
		{
			ImGui::Begin("Particle System");

			static float f = 0.0f;
			ImGui::Checkbox("Active", &RenderModule::allRenderers[2]->m_Active);
			ImGui::Text("Edit Text Props");                           // Display some text (you can use a format string too)
			ImGui::DragFloat3("pos", (float*)&RenderModule::allRenderers[2]->transform.position, 0.01f);
			ImGui::DragFloat3("size", (float*)&RenderModule::allRenderers[2]->transform.scale, 0.01f);
			ImGui::SliderFloat("rot", &f, -180.0f, 180.0f, "%.1f", 1.0f);
			RenderModule::allRenderers[2]->transform.rotation = Quaternion::AngleAxis(f, Vector3::forward());

			ImGui::End();
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
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			HDC backup_current_context = RenderModule::openGLSystem->GetWindowContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			wglMakeCurrent(backup_current_context, RenderModule::openGLSystem->GetGLContext());
		}
		// ----------------
		// ImGui render end
		// -----------------
	}
}
