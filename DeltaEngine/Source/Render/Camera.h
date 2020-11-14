#pragma once

#include "DEpch.h"
#include "FrameBuffer.h"
#include "Material.h"
#include "Components/Transform.h"
#include "Core/Math/Color.h"

namespace DeltaEngine
{
  class Camera
  {
    int cameraIndex;
    FrameBuffer frameBuffer;
    float m_AspectRatio, m_ViewportSize;
  public:
    static std::vector<Camera*> allCameras;
    // camera for editor mode only
    static Camera* editorCamera;
    float m_Size;
    float m_zNear, m_zFar;
    Color backgroundColor;
    Shader* shader;

    Transform transform;
    Camera(bool editor = false);
    ~Camera();
    Matrix4x4 GetProjectionMatrix() const;
    Matrix4x4 GetViewMatrix() const;
    Vector3 Max() const;
    Vector3 Min() const;
    FrameBuffer& GetFrameBuffer();
    float GetAspectRatio();
    float SetAspectRatio(float width, float height);
    float SetViewportSize(float width);

    void Start();
    void End();
  };
}
