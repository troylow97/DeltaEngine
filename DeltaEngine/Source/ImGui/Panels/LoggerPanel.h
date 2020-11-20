#pragma once

#include <spdlog/spdlog.h>
#include "ImGui/Panels/IPanel.h"
#include "Core/Containers/Singleton.h"

namespace DeltaEngine
{
	class LoggerPanel : public IPanel
	{
    public:
        ImGuiTextBuffer m_logger_buffer;
        ImGuiTextFilter m_logger_filter;
        ImVector<int>   m_logger_line_offsets; // index to lines offset, maintained by AddLog() calls
        bool            m_logger_auto_scroll;  // keep scrolling if already at the bottom

        LoggerPanel(std::string str);
        ~LoggerPanel();
        bool DraggedFileIn();
        void Render(bool) override;
        void Clear();
        //void AddLog(const char* fmt, ...) IM_FMTARGS(2);

    private:
        ImVec2 topLeft;
        ImVec2 bottomRight;
	};

    class ImGuiLogger : public Singleton<ImGuiLogger>
    {
        friend class Singleton<ImGuiLogger>;
    public:
        inline static ImGuiTextBuffer m_logger_buffer;
        inline static ImVector<int>   m_logger_line_offsets; // index to lines offset, maintained by AddLog() calls

        //static void AddLog(const char* fmt, Event* e, ...) IM_FMTARGS(2);
    };
}