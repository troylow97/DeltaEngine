#pragma once

#include "ImGui/Panels/IPanel.h"
#include "Audio/AudioConfig.h"
namespace DeltaEngine
{
  class AudioPanel : public IPanel
  {
    AudioConfig config;

  public:

    AudioPanel(std::string str, Editor& e);
    ~AudioPanel();
    void Render() override;
  };
}
