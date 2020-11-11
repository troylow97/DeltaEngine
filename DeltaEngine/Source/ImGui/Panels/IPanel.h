#pragma once

#include <string>
#include <imgui.h>

namespace DeltaEngine
{
	class IPanel
	{
	public:
		virtual void Render(bool) = 0;
		IPanel(std::string str) : m_name(str) { }
		virtual ~IPanel() = default;
		void Enable() { m_enabled = !m_enabled; }
		bool IsEnabled() const { return m_enabled; }
		std::string Name() const { return m_name; }

		//virtual bool DraggedFileIn(Event* e) = 0;

		ImVec2 GetTopLeft() { return topLeft; }
		ImVec2 GetBottomRight() { return topLeft; }

	protected:
		bool m_enabled{ false };
		std::string m_name;
		ImVec2 topLeft;
		ImVec2 bottomRight;
	};
}