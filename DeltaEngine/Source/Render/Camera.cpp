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
  FrameBuffer* Camera::finalFrameBuffer;
  Camera* Camera::editorCamera;
  Transform Camera::editorCameraTransform;
  float Camera::fixedAspect = 16.f / 9.f;

  Camera::Camera(bool editor) :
    cameraIndex{ editor ? -1 : static_cast<int>(allCameras.size()) },
    frameBuffer{},
    aspectRatio{ 16.f / 9.f },
    viewportSize{ 1920 },
    camWidth{ 1920 },
    m_Size{ 6 },
    m_FieldOfView{ 60 },
    m_zNear{ .3f },
    m_zFar{ 1000 },
    backgroundColor{ 49 / 255.0f, 77 / 255.0f, 121 / 255.0f, 1 },
    perspective{true}
  {
    if (!editor)
      allCameras.push_back(this);
    else
    {
      if (!editorCamera)
      {
        editorCamera = this;
        editorCameraTransform = Transform();
        perspective = false;
      }
      else
      {
        DeltaEngine_CORE_ERROR("An editor camera already exists, only one should exist at a time.");
        delete this;
      }
    }
  }
  Camera::Camera(const Camera& copy) :
    cameraIndex{ static_cast<int>(allCameras.size()) },
    frameBuffer{},
    aspectRatio{ 16.f / 9.f },
    viewportSize{ 1920 },
    camWidth{ 1920 },
    m_Size{ copy.m_Size },
    m_FieldOfView{ copy.m_FieldOfView },
    m_zNear{ copy.m_zNear },
    m_zFar{ copy.m_zFar },
    backgroundColor{ copy.backgroundColor },
    perspective{ copy.perspective }
  {
    if (&copy == editorCamera)
    {
      DeltaEngine_CORE_ERROR("Copying the editor camera is not allowed!");
      delete this;
    }
    else
      allCameras.push_back(this);
  }

  Camera::Camera(Camera&& move) :
    cameraIndex{ static_cast<int>(allCameras.size()) },
    frameBuffer{},
    aspectRatio{ 16.f / 9.f },
    viewportSize{ 1920 },
    camWidth{ 1920 },
    m_Size{ move.m_Size },
    m_FieldOfView{ move.m_FieldOfView },
    m_zNear{ move.m_zNear },
    m_zFar{ move.m_zFar },
    backgroundColor{ move.backgroundColor },
    perspective{ move.perspective }
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
    m_Size = copy.m_Size;
    m_FieldOfView = copy.m_FieldOfView;
    m_zNear = copy.m_zNear;
    m_zFar = copy.m_zFar;
    backgroundColor = copy.backgroundColor;
    perspective = copy.perspective;
    return *this;
  }

  Camera& Camera::operator=(Camera&& move)
  {
    // moving camera should not be allowed, just copy
    if (&move == editorCamera)
    {
      DeltaEngine_CORE_ERROR("Copying the editor camera is not allowed!");
      return *this;
    }
    m_Size = move.m_Size;
    m_FieldOfView = move.m_FieldOfView;
    m_zNear = move.m_zNear;
    m_zFar = move.m_zFar;
    backgroundColor = move.backgroundColor;
    perspective = move.perspective;
    return *this;
  }

  Matrix4x4 Camera::GetOrthographicMatrix(Transform transform) const
  {
    return Matrix4x4::Rotate(transform.rotation) * Matrix4x4::Ortho(
      -m_Size * aspectRatio,
      m_Size * aspectRatio,
      -m_Size, m_Size, -1, 1);
  }
  Matrix4x4 Camera::GetPerspectiveMatrix(Transform transform) const
  {
    return Matrix4x4::Rotate(transform.rotation) * Matrix4x4::Perspective(
      60,
      (fixedAspect > .01f) ? fixedAspect : aspectRatio,
      m_zNear, m_zFar);
  }

  Matrix4x4 Camera::GetProjectionMatrix(Transform transform)
  {
    if (m_zNear > m_zFar)
      m_zFar = m_zNear;
    if (perspective)
      return GetPerspectiveMatrix(transform);
    else
      return GetOrthographicMatrix(transform);
  }

  Matrix4x4 Camera::GetViewMatrix(Transform transform) const
  {
    return Matrix4x4::Transpose(Matrix4x4::Translate(-transform.position));
  }

  Vector3 Camera::Max(Transform transform) const
  {
    return Vector3(m_Size * aspectRatio + transform.position.x, m_Size + transform.position.y);
  }

  Vector3 Camera::Min(Transform transform) const
  {
    return Vector3(-m_Size * aspectRatio + transform.position.x, -m_Size + transform.position.y);
  }

  FrameBuffer& Camera::GetFrameBuffer()
  {
    return frameBuffer;
  }

  float Camera::GetAspectRatio()
  {
    return aspectRatio;
  }

  float Camera::SetAspectRatio(float width, float height)
  {
    aspectRatio = width / height;
    return aspectRatio;
  }

  float Camera::GetFixedAspectRatio()
  {
    return fixedAspect;
  }

  float Camera::SetFixedAspectRatio(float width, float height)
  {
    fixedAspect = width / height;
    return fixedAspect;
  }

  float Camera::GetViewportSize()
  {
    return camWidth;
  }

  float Camera::SetViewportSize(float width)
  {
    viewportSize = width;
    camWidth = viewportSize;
    if (fixedAspect > .01f && aspectRatio > fixedAspect)
      camWidth *= fixedAspect / aspectRatio;
    return camWidth;
  }

  float Camera::GetTrueViewportSize()
  {
    return viewportSize;
  }

  void Camera::Start()
  {
    frameBuffer.Resize(static_cast<unsigned>(camWidth),
      static_cast<unsigned>(camWidth / fixedAspect));
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

  void Camera::Shake(float magnitude, float fade)
  {
    shakeMagnitude = magnitude;
    shakeFade = fade;
  }
}
