#include "Camera.h"
#include <algorithm>
#include "Window.h"
#include "Mesh.h"
#include "OpenGLSystem.h"
#include <GL/glew.h>
#include "Core/Debugging/Gizmos.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
std::vector<Camera *> Camera::allCameras;
Camera *Camera::editorCamera;
Camera::Camera( bool editor ) :
  cameraIndex { editor ? -1 : static_cast<int>( allCameras.size() ) }, frameBuffer {},
  m_AspectRatio { 1.0f * env.pWin->Width() / env.pWin->Height() }, m_ViewportSize { 1.0f * env.pWin->Width() },
  m_Size { 5 }, m_zNear { -100 }, m_zFar { 100 },
  backgroundColor { 49 / 255.0f, 77 / 255.0f, 121 / 255.0f, 1 },
  shader { new Shader( "Shaders/DefaultScreen" ) }
{
  if ( !editor )
    allCameras.push_back( this );
  else
  {
    if ( !editorCamera )
      editorCamera = this;
    else
    {
      DeltaEngine_CORE_ERROR( "An editor camera already exists, only one should exist at a time" );
      delete this;
    }
  }
}
Camera::~Camera()
{
  if ( cameraIndex >= 0 )
  {
    allCameras.erase( allCameras.begin() + cameraIndex );

    for ( int i = 0; i < allCameras.size(); ++i )
      allCameras[i]->cameraIndex = i;
  }
}
Matrix4x4 Camera::GetProjectionMatrix() const
{
  return Matrix4x4::Rotate( transform.rotation ) * Matrix4x4::Ortho(
    -m_Size * m_AspectRatio,
    m_Size * m_AspectRatio,
    -m_Size, m_Size, m_zNear, m_zFar );
}
Matrix4x4 Camera::GetViewMatrix() const
{
  return Matrix4x4::Transpose( Matrix4x4::Translate( -transform.position ) );;
}
Vector3 Camera::Max() const
{
  return Vector3( m_Size * m_AspectRatio + transform.position.x, m_Size + transform.position.y );
}
Vector3 Camera::Min() const
{
  return Vector3( -m_Size * m_AspectRatio + transform.position.x, -m_Size + transform.position.y );
}
FrameBuffer &Camera::GetFrameBuffer()
{
  return frameBuffer;
}
float Camera::GetAspectRatio()
{
  return m_AspectRatio;
}
float Camera::SetAspectRatio( float width, float height )
{
  return m_AspectRatio = width / height;
}
float Camera::SetViewportSize( float width )
{
  return m_ViewportSize = width;
}

void Camera::Start()
{
  frameBuffer.Resize( static_cast<unsigned int>( m_ViewportSize ),
                      static_cast<unsigned int>( m_ViewportSize / m_AspectRatio ) );

  frameBuffer.Bind();

  //glEnable(GL_DEPTH_TEST);
  glClearColor(
    backgroundColor.r,
    backgroundColor.g,
    backgroundColor.b,
    backgroundColor.a );
  glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);
}
void Camera::End()
{
  if ( this == editorCamera )
  {
    Gizmos::DrawWorldGrid();
  }
  frameBuffer.Unbind();
  //glDisable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT);
}
}

