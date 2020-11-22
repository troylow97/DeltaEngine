#include "ImGui/Panels/LoggerPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
//#include "Render/Texture.h"
//#include "Assets/AssetManager.h"
//#include "ECS/ECSModule.h"
//#include "Core/Utils/FileUtils.h"
#include "ECS/ECSModule.h"
#include "Event/ApplicationEvent.h"

namespace DeltaEngine
{
  LoggerPanel::LoggerPanel(std::string str) :
      IPanel(str)
  {
    m_enabled = true;
    m_logger_auto_scroll = true;
  }
  
  LoggerPanel::~LoggerPanel()
  {
    m_enabled = false;
  }
  
  bool LoggerPanel::DraggedFileIn()
  {
    if (InputManager::Instance().CurrentPosition().point_x >= GetTopLeft().x && InputManager::Instance().
        CurrentPosition().point_x <= GetBottomRight().x
        && InputManager::Instance().CurrentPosition().point_y >= GetTopLeft().y && InputManager::Instance().
        CurrentPosition().point_y <= GetBottomRight().y)
    {
      std::cout << "it is in logger panel!!!" << std::endl;
      return true;
    }
    return false;
  }
  
  void LoggerPanel::Render(bool isdragged)
  {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None; // ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        //ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse/* | ImGuiWindowFlags_NoSavedSettings*/;
    ImGui::Begin(m_name.c_str(), &m_enabled, window_flags);
    
    topLeft = ImGui::GetWindowContentRegionMin();
    bottomRight = ImGui::GetWindowContentRegionMax();
    
    topLeft.x += ImGui::GetWindowPos().x;
    topLeft.y += ImGui::GetWindowPos().y;
    bottomRight.x += ImGui::GetWindowPos().x;
    bottomRight.y += ImGui::GetWindowPos().y;
    
    if (ImGui::BeginPopup("Options"))
    {
      ImGui::Checkbox("Auto Scroll", &m_logger_auto_scroll);
      ImGui::EndPopup();
    }
    
    if (ImGui::Button("Options"))
    {
      ImGui::OpenPopup("Options");
    }
    ImGui::SameLine();
    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    m_logger_filter.Draw("", 250.0f);
    
    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    
    if (clear)
      Clear();
    if (copy)
      ImGui::LogToClipboard();
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    const char* buffer_begin = m_logger_buffer.begin();
    const char* bufffer_end = m_logger_buffer.end();
    
    if (m_logger_filter.IsActive())
    {
      for (int line_number = 0; line_number < m_logger_line_offsets.Size; line_number++)
      {
        const char* line_start = buffer_begin + m_logger_line_offsets[line_number];
        const char* line_end = (line_number + 1 < m_logger_line_offsets.Size) ? (buffer_begin + m_logger_line_offsets[line_number + 1] - 1) : bufffer_end;
        if (m_logger_filter.PassFilter(line_start, line_end))
            ImGui::TextUnformatted(line_start, line_end);
      }
    }
    else
    {
      ImGuiListClipper clipper;
      clipper.Begin(m_logger_line_offsets.Size);
      while (clipper.Step())
      {
        for (int line_number = clipper.DisplayStart; line_number < clipper.DisplayEnd; line_number++)
        {
          const char* line_start = buffer_begin + m_logger_line_offsets[line_number];
          const char* line_end = (line_number + 1 < m_logger_line_offsets.Size) ? (buffer_begin + m_logger_line_offsets[line_number + 1] - 1) : bufffer_end;
          ImGui::TextUnformatted(line_start, line_end);
        }
      }
      clipper.End();
    }
    ImGui::PopStyleVar();
    
    if (m_logger_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    
    ImGui::EndChild();

    ImGuiLogger::Instance().WriteLog(m_logger_buffer, m_logger_line_offsets);
    ImGui::End();
  }
  
  void LoggerPanel::Clear()
  {
    m_logger_buffer.clear();
    m_logger_line_offsets.clear();
    m_logger_line_offsets.push_back(0);
  }
  
  //void LoggerPanel::AddLog(const char* fmt, ...) IM_FMTARGS(2)
  //{
  //    int previous_size = m_logger_buffer.size();
  //    va_list args;
  //    va_start(args, fmt);
  //    m_logger_buffer.appendfv(fmt, args);
  //    va_end(args);
  //
  //    for (int current_size = m_logger_buffer.size(); previous_size < current_size; ++previous_size)
  //    {
  //        if (m_logger_buffer[previous_size] == '\n')
  //        {
  //            m_logger_line_offsets.push_back(previous_size + 1);
  //        }
  //    }
  //}
  
  void ImGuiLogger::AddLog(const char* fmt, ...) IM_FMTARGS(2)
  {
    int previous_size = m_logger_buffer.size();
    va_list args;
    va_start(args, fmt);
    m_logger_buffer.appendfv(fmt, args);
    va_end(args);
    
    for (int current_size = m_logger_buffer.size(); previous_size < current_size; ++previous_size)
    {
      if (m_logger_buffer[previous_size] == '\n')
      {
        m_logger_line_offsets.push_back(previous_size + 1);
      }
    }
  }
  void ImGuiLogger::WriteLog(ImGuiTextBuffer _buffer, ImVector<int> _lineoffsets)
  {
    _buffer = m_logger_buffer;
    _lineoffsets = m_logger_line_offsets;
  }
}
