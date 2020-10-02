#include "ImGuiLayer.h"
#include "Render/Window.h"
#include "Render/OpenGLSystem.h"
#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_opengl3.h>

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
		ImGui_ImplWin32_Init(RenderModule::mainHWND, RenderModule::openGLSystem->GetGLContext());
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

	void ImGuiLayer::OnEvent(Event& e)
	{

	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_Shutdown();
		ImGui::NewFrame();
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
