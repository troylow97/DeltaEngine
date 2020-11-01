#pragma once

#include <string>

#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_opengl3.h>

namespace DeltaEngine
{
	class IPanel
	{
	public:
		virtual bool Render() = 0;
		IPanel(std::string str) : m_name(str) { }
		void Enable(bool = true) { m_enabled; }
		bool IsEnabled() const { return m_enabled; }
		std::string Name() const { return m_name; }

		virtual bool DraggedFileIn() = 0;



		virtual float GetHeight() = 0;
		virtual float GetWidth() = 0;
		virtual ImVec2 GetTopLeft() = 0;
		virtual ImVec2 GetBottomRight() = 0;

	protected:
		bool m_enabled{ false };
		std::string m_name;
	};
}