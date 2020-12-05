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
  };
}
