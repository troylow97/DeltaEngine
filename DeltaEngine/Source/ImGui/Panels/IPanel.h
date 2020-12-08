/**********************************************************************************
* \file   IPanel.h
* \brief  The file contains IPanel declaration as base class for all panels.
* \author Chin, Clara,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <string>


#include "Core/Debugging/Logger/Log.h"
#include "ImGui/Editor.h"

namespace DeltaEngine
{
  class IPanel
  {
  public:
    virtual void Render() = 0;

    IPanel(std::string str, Editor& e) : m_name(str), m_editor(e)
    {
      DeltaEngine_CORE_INFO( "Editor created {} panel", m_name );
    }

    virtual ~IPanel() = default;
    virtual void Enable() { m_enabled = !m_enabled; }
    bool IsEnabled() const { return m_enabled; }
    bool IsActive() const
    {
      return m_active;
    }
    const std::string& Name() const { return m_name; }

  protected:
    Editor &m_editor;
    std::string m_name;
    bool m_enabled{false};
    bool m_active{false};

  };
}
