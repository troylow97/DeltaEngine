/**********************************************************************************
* \file   LoggerPanel.h
* \brief  The file contains interface of LoggerPanel.
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <spdlog/spdlog.h>

#include "imgui.h"
#include "ImGui/Panels/IPanel.h"
#include "Core/Containers/Singleton.h"

namespace DeltaEngine
{
  //struct Event;

  class LoggerPanel : public IPanel
  {
  public:
    ImGuiTextBuffer m_logger_buffer;
    ImGuiTextFilter m_logger_filter;
    ImVector<int> m_logger_line_offsets; // index to lines offset, maintained by AddLog() calls
    bool m_logger_auto_scroll; // keep scrolling if already at the bottom

    LoggerPanel(std::string str, Editor& e);
    ~LoggerPanel();
    void Render() override;
    void Clear();
  };

  class ImGuiLogger : public Singleton<ImGuiLogger>
  {
    friend class Singleton<ImGuiLogger>;
  public:
    inline static ImGuiTextBuffer m_logger_buffer;
    inline static ImVector<int> m_logger_line_offsets; // index to lines offset, maintained by AddLog() calls

    static void AddLog(const char* fmt, ...) IM_FMTARGS(2);
    static void WriteLog(ImGuiTextBuffer _buffer, ImVector<int> _lineoffsets);
  };
}
