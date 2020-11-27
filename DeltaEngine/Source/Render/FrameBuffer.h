#pragma once

#include "Core/Math/Vector.h"

namespace DeltaEngine
{
  class FrameBuffer
  {
    const unsigned int s_MaxFramebufferSize = 8192;
    unsigned int m_RendererID = 0;
    unsigned int m_ColorAttachment = 0, m_DepthAttachment = 0;
    unsigned int m_Width = 256, m_Height = 256;
  public:
    FrameBuffer();
    FrameBuffer(const FrameBuffer&);
    FrameBuffer(FrameBuffer&&);
    ~FrameBuffer();
    FrameBuffer& operator=(const FrameBuffer&);
    FrameBuffer& operator=(FrameBuffer&&);
    unsigned int GetColorAttachment() const;
    unsigned int GetDepthAttachment() const;
    void Bind() const;
    void Unbind() const;
    void Invalidate();
    void Resize(unsigned int width, unsigned int height);
  };
}
