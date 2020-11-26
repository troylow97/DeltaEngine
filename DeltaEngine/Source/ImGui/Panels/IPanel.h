#pragma once

#include <string>
#include <imgui.h>

namespace DeltaEngine
{
  class IPanel
  {
  public:
    virtual void Render() = 0;

    IPanel(std::string str) : m_name(str)
    {
    }

    virtual ~IPanel() = default;
    void Enable() { m_enabled = !m_enabled; }
    bool IsEnabled() const { return m_enabled; }
    const std::string& Name() const { return m_name; }

  protected:
    bool m_enabled{false};
    std::string m_name;
  };
}
