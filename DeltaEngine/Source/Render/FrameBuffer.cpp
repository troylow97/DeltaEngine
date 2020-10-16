#include "FrameBuffer.h"
#include "DEpch.h"
#include "Window.h"
#include "ErrorCheck.h"
#include <GL/glew.h>

namespace DeltaEngine
{
FrameBuffer::FrameBuffer()
{
  glGenFramebuffers( 1, &m_RendererID );

  Invalidate();
}
FrameBuffer::~FrameBuffer()
{
  GLCall( glDeleteFramebuffers( 1, &m_RendererID ) );
  GLCall( glDeleteTextures( 1, &m_ColorAttachment ) );
  GLCall( glDeleteTextures( 1, &m_DepthAttachment ) );
}

unsigned int FrameBuffer::GetColorAttachment() const
{
  return m_ColorAttachment;
}
unsigned int FrameBuffer::GetDepthAttachment() const
{
  return m_DepthAttachment;
}

void FrameBuffer::Bind() const
{
  GLCall( glBindFramebuffer( GL_FRAMEBUFFER, m_RendererID ) );
  GLCall( glViewport( 0, 0, _width, _height ) );
}
void FrameBuffer::Unbind() const
{
  GLCall( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
}
void FrameBuffer::Invalidate()
{
  if ( m_RendererID )
  {
    GLCall( glDeleteFramebuffers( 1, &m_RendererID ) );
    GLCall( glDeleteTextures( 1, &m_ColorAttachment ) );
    GLCall( glDeleteTextures( 1, &m_DepthAttachment ) );
  }

  GLCall( glCreateFramebuffers( 1, &m_RendererID ) );
  GLCall( glBindFramebuffer( GL_FRAMEBUFFER, m_RendererID ) );

  GLCall( glCreateTextures( GL_TEXTURE_2D, 1, &m_ColorAttachment ) );
  GLCall( glBindTexture( GL_TEXTURE_2D, m_ColorAttachment ) );
  GLCall( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );

  GLCall( glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0 ) );

  GLCall( glCreateTextures( GL_TEXTURE_2D, 1, &m_DepthAttachment ) );
  GLCall( glBindTexture( GL_TEXTURE_2D, m_DepthAttachment ) );
  GLCall( glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _width, _height ) );
  GLCall( glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0 ) );

  ASSERT_ERROR( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!" );

  GLCall( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
}
void FrameBuffer::Resize( unsigned int width, unsigned int height )
{
  if ( width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize )
  {
    DeltaEngine_CORE_WARN( "Attempted to rezize framebuffer to {0}, {1}", width, height );
    return;
  }
  _width = width;
  _height = height;
  Invalidate();
}
}
