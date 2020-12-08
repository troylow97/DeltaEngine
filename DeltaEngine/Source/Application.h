/**********************************************************************************
* \file   Application.h
* \brief  The file contains code for running the engine application
* \author Chin, Clara,   25% Code Contribution
* \author Low, Troy,     25% Code Contribution
* \author Ong, Graeme,   25% Code Contribution
* \author Tan, Tong Wee, 25% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Event/ApplicationEvent.h"
#include "ImGui/Editor.h"

namespace DeltaEngine
{
  class Application
  {
  public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent();
    bool OnWindowResize(WindowResizeEvent& e);
  };

  Application* CreateApplication();
}
