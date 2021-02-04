/**********************************************************************************
* \file   GamePanel.h
* \brief  The file contains interface of GamePanel.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
  class GamePanel : public IPanel
  {
  public:
    GamePanel(std::string str, Editor& e);
    ~GamePanel();
    void Render() override;
    static inline Vector2 render_pos;
    static inline Vector2 render_size;
    static inline Vector2 mouse_pos;
  };
}
