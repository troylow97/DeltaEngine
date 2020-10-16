#pragma once

#include "Core/Layer.h"

namespace DeltaEngine
{
class ImGuiLayer : public Layer
{
public:
  ImGuiLayer();
  ~ImGuiLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent() override;

  void Begin();
  void End();

  void BlockEvents( bool block )
  {
    m_BlockEvents = block;
  }
private:
  bool m_BlockEvents = true;
  float m_Time = 0.0f;
};
}
