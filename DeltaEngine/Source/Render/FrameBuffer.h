#pragma once

#include "Core/Math/Vector.h"

namespace DeltaEngine
{
  class FrameBuffer
  {
    unsigned int m_RendererID = 0;
    unsigned int m_ColorAttachment = 0, m_DepthAttachment = 0;
    unsigned int _width = 256, _height = 256;
    const unsigned int s_MaxFramebufferSize = 8192;
  public:
    FrameBuffer();
    ~FrameBuffer();
    unsigned int GetColorAttachment() const;
    unsigned int GetDepthAttachment() const;
    void Bind() const;
    void Unbind() const;
    void Invalidate();
    void Resize(unsigned int width, unsigned int height);
  };
}
