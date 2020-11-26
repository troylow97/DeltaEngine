#pragma once

#include "Editor.h"

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_win32.h"

// Core
#include "Core/GlobalStruct.h"
#include "Core/Utils/FileDialog.h"

// Input
#include "Input/InputManager.h"

// Render
#include "Render/OpenGLSystem.h"
#include "Render/Window.h"

// ECS
#include "ECS/ECSModule.h"

// Panels
#include "Panels/AssetPanel.h"
#include "Panels/GamePanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/PropertyInspectorPanel.h"
#include "Panels/TilemapPanel.h"
#include "Panels/WorldPanel.h"
#include "Panels/SpriteEditorPanel.h"

#include "EditorDirectoryWatcher.h"
#include "IconsFontAwesome5.h"
#include "Core/TypeAlias.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/Keys.h"
#include "Panels/ButtonsPanel.h"

namespace DeltaEngine
{
  void NewFile()
  {
    GetEnv().pECS->GetWorld().GetEntityManager().Clear();
    InputManager::Instance().SetEntitySelected(false);
    InputManager::Instance().SetEntityIDSelected(u64_max);
  }

  void OpenFile()
  {
    std::optional<std::string> path = FileDialogs::OpenFile("DeltaEngine Scene (*.json)\0*.json\0");

    if (path)
    {
      NewFile();
      GetEnv().pECS->GetWorld().Load(*path);
    }
  }

  void SaveFile()
  {
    std::optional<std::string> path = FileDialogs::SaveFile("DeltaEngine Scene (*.json)\0*\0");

    if (path)
      GetEnv().pECS->GetWorld().Save(*path);
  }

  void AddEntity()
  {
    GetEnv().pECS->GetWorld().GetEntityManager().CreateEntity<Transform>();
  }

  void CreateEntityFromArchetype()
  {
    if (InputManager::Instance().EntitySelected())
      GetEnv().pECS->GetWorld().GetEntityManager().CreateEntityFromArchetype({
        InputManager::Instance().EntityIDSelected()
      });
  }

  void CloneEntity()
  {
    if (InputManager::Instance().EntitySelected())
      GetEnv().pECS->GetWorld().GetEntityManager().CloneEntity({InputManager::Instance().EntityIDSelected()});
  }

  void DeleteEntity()
  {
    auto id = InputManager::Instance().EntityIDSelected();
    GetEnv().pECS->GetWorld().GetEntityManager().DestroyEntity({id});
    InputManager::Instance().SetEntitySelected(false);
    InputManager::Instance().SetEntityIDSelected(u64_max);
    DeltaEngine_CORE_TRACE("Deleted Entity - {}", id);
  }

  void Editor::MenuBar()
  {
    if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyReleased(DEVK_N))
      NewFile();
    if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyReleased(DEVK_O))
      OpenFile();
    if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyReleased(DEVK_S))
      SaveFile();
    if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyDown(DEVK_LSHIFT) && ImGui::IsKeyReleased(DEVK_A))
      AddEntity();
    if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyDown(DEVK_LSHIFT) && ImGui::IsKeyReleased(DEVK_C))
      CreateEntityFromArchetype();
    if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyDown(DEVK_LSHIFT) && ImGui::IsKeyReleased(DEVK_V))
      CloneEntity();
    if (ImGui::IsKeyReleased(DEVK_DELETE) && InputManager::Instance().EntitySelected() && InputManager::Instance().
      EntityIDSelected() != u64_max)
      DeleteEntity();

    if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyReleased(DEVK_W))
      m_panels[2]->Enable();
    if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyReleased(DEVK_I))
      m_panels[1]->Enable();
    if (ImGui::IsKeyDown(DEVK_LCTRL) && ImGui::IsKeyReleased(DEVK_T))
      m_panels[0]->Enable();

    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("Scene"))
      {
        if (ImGui::MenuItem("New", " Ctrl+N"))
          NewFile();
        if (ImGui::MenuItem("Open", " Ctrl+O"))
          OpenFile();
        if (ImGui::MenuItem("Save", " Ctrl+S"))
          SaveFile();
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Entities"))
      {
        if (ImGui::MenuItem("Add Entity", " Ctrl+Shift+A"))
          AddEntity();
        if (ImGui::MenuItem("Clone Entity", " Ctrl+Shift+V"))
          CloneEntity();
        if (ImGui::MenuItem("Clone Entity Default", " Ctrl+Shift+C"))
          CreateEntityFromArchetype();
        if (ImGui::MenuItem("Delete Entity", " Del"))
          DeleteEntity();

        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View"))
      {
        if (ImGui::MenuItem("World", " Ctrl+W"))
          m_panels[2]->Enable();
        if (ImGui::MenuItem("Inspector", " Ctrl+I"))
          m_panels[1]->Enable();
        if (ImGui::MenuItem("Tiles", " Ctrl+T"))
          m_panels[0]->Enable();
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

  Editor::Editor()
  {
    m_panels.push_back(std::make_unique<TilemapPanel>("Tilemap"));
    m_panels.push_back(std::make_unique<PropertyInspectorPanel>("Property Inspector"));
    m_panels.push_back(std::make_unique<WorldPanel>("World"));
    m_panels.push_back(std::make_unique<ViewportPanel>("Viewport"));
    m_panels.push_back(std::make_unique<AssetPanel>("Assets"));
    m_panels.push_back(std::make_unique<ButtonsPanel>("Buttons"));
    //m_panels.push_back(std::make_unique<SpriteEditorPanel>("Sprite Editor"));
    //m_panels.push_back(std::make_unique<GamePanel>("Game"));


    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable |
      ImGuiConfigFlags_ViewportsEnable;

    io.Fonts->AddFontDefault();
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.GlyphMinAdvanceX = 16.0f; // Use if you want to make the icon monospaced
    icons_config.PixelSnapH = true;
    // add character ranges and merge into main font, merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", 10.0f, &icons_config, icons_ranges);
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(env.pWin->GetHandle(), RenderModule::openGLSystem->GetGLContext());
    ImGui_ImplOpenGL3_Init("#version 410");

    //EditorDirectoryWatcher::Instance().Initialize();
  }

  Editor::~Editor()
  {
    //EditorDirectoryWatcher::Instance().Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
  }

  void Editor::Begin()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;

    // Update ImGui with Custom Input
    auto& ref = ImGui::GetIO();
    std::memcpy(&ref.KeysDown[0], InputManager::Instance().GetKeys(), 256);

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
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;
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
  }

  void Editor::Render()
  {
    MenuBar();

    for (auto& ref : m_panels)
      if (ref->IsEnabled())
        ref->Render(drag); // update all the panels
  }

  void Editor::End()
  {
    ImGui::End();
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
    Profiler::Instance().Record("ImGui");
  }

  void Editor::OnDragDrop(Event* e)
  {
    drag = true;
  }

  void Editor::OnRemovingDragDrop(Event* e)
  {
    drag = false;
  }

  void Editor::OnDragDropDone(Event* e)
  {
    drag = false;
  }
};
