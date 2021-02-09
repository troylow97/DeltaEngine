/**********************************************************************************
* \file   Camera.cpp
* \brief  The file contains implementation of Camera class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
  std::vector<Camera*> Camera::allCameras;
  Camera* Camera::editorCamera;
  Transform Camera::editorCameraTransform;

  Camera::Camera(bool editor) :
    cameraIndex{editor ? -1 : static_cast<int>(allCameras.size())},
    frameBuffer{},
    m_AspectRatio{1.0f * env.pWin->Width() / env.pWin->Height()},
    m_ViewportSize{1.0f * env.pWin->Width()},
    m_Size{6},
    m_zNear{-100},
    m_zFar{100},
    backgroundColor{49 / 255.0f, 77 / 255.0f, 121 / 255.0f, 1}
  {
    if (!editor)
      allCameras.push_back(this);
    else
    {
      if (!editorCamera)
      {
        editorCamera = this;
        editorCameraTransform = Transform();
      }
      else
      {
        DeltaEngine_CORE_ERROR("An editor camera already exists, only one should exist at a time.");
        delete this;
      }
    }
  }

  Camera::Camera(const Camera& copy) :
    cameraIndex{static_cast<int>(allCameras.size())},
    frameBuffer{},
    m_AspectRatio{copy.m_AspectRatio},
    m_ViewportSize{copy.m_ViewportSize},
    m_Size{copy.m_Size},
    m_zNear{copy.m_zNear},
    m_zFar{copy.m_zFar},
    backgroundColor{copy.backgroundColor}
  {
    if (&copy == editorCamera)
    {
      DeltaEngine_CORE_ERROR("Copying the editor camera is not allowed!");
      delete this;
    }
    else
      allCameras.push_back(this);
  }

  Camera::Camera(Camera&& move) noexcept :
    cameraIndex{static_cast<int>(allCameras.size())},
    frameBuffer{},
    m_AspectRatio{move.m_AspectRatio},
    m_ViewportSize{move.m_ViewportSize},
    m_Size{move.m_Size},
    m_zNear{move.m_zNear},
    m_zFar{move.m_zFar},
    backgroundColor{move.backgroundColor}
  {
    if (&move == editorCamera)
    {
      DeltaEngine_CORE_ERROR("Copying the editor camera is not allowed!");
      delete this;
    }
    else
      allCameras.push_back(this);
  }

  Camera::~Camera()
  {
    if (cameraIndex >= 0)
    {
      allCameras.erase(allCameras.begin() + cameraIndex);

      for (int i = 0; i < allCameras.size(); ++i)
        allCameras[i]->cameraIndex = i;
    }
  }

  Camera& Camera::operator=(const Camera& copy)
  {
    if (&copy == editorCamera)
    {
      DeltaEngine_CORE_ERROR("Copying the editor camera is not allowed!");
      return *this;
    }
    m_AspectRatio = copy.m_AspectRatio;
    m_ViewportSize = copy.m_ViewportSize;
    m_Size = copy.m_Size;
    m_zNear = copy.m_zNear;
    m_zFar = copy.m_zFar;
    backgroundColor = copy.backgroundColor;
    return *this;
  }

  Camera& Camera::operator=(Camera&& move) noexcept
  {
    // moving camera should not be allowed, just copy
    if (&move == editorCamera)
    {
      DeltaEngine_CORE_ERROR("Copying the editor camera is not allowed!");
      return *this;
    }
    m_AspectRatio = move.m_AspectRatio;
    m_ViewportSize = move.m_ViewportSize;
    m_Size = move.m_Size;
    m_zNear = move.m_zNear;
    m_zFar = move.m_zFar;
    backgroundColor = move.backgroundColor;
    return *this;
  }

  Matrix4x4 Camera::GetProjectionMatrix(Transform transform) const
  {
    return Matrix4x4::Rotate(transform.rotation) * Matrix4x4::Ortho(
      -m_Size * m_AspectRatio,
      m_Size * m_AspectRatio,
      -m_Size, m_Size, m_zNear, m_zFar);
  }

  Matrix4x4 Camera::GetViewMatrix(Transform transform) const
  {
    return Matrix4x4::Transpose(Matrix4x4::Translate(-transform.position));
  }

  Vector3 Camera::Max(Transform transform) const
  {
    return Vector3(m_Size * m_AspectRatio + transform.position.x, m_Size + transform.position.y);
  }

  Vector3 Camera::Min(Transform transform) const
  {
    return Vector3(-m_Size * m_AspectRatio + transform.position.x, -m_Size + transform.position.y);
  }

  FrameBuffer& Camera::GetFrameBuffer()
  {
    return frameBuffer;
  }

  float Camera::GetAspectRatio()
  {
    return m_AspectRatio;
  }

  float Camera::SetAspectRatio(float width, float height)
  {
    return m_AspectRatio = width / height;
  }

  float Camera::SetViewportSize(float width)
  {
    return m_ViewportSize = width;
  }

  void Camera::Start()
  {
    frameBuffer.Resize(static_cast<unsigned int>(m_ViewportSize),
                       static_cast<unsigned int>(m_ViewportSize / m_AspectRatio));

    frameBuffer.Bind();

    //glEnable(GL_DEPTH_TEST);
    glClearColor(
      backgroundColor.r,
      backgroundColor.g,
      backgroundColor.b,
      backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);
  }

  void Camera::End()
  {
    if (this == editorCamera)
      Gizmos::DrawWorldGrid();
    frameBuffer.Unbind();
    //glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
  }
}
