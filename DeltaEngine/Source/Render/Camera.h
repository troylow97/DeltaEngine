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
    static Transform editorCameraTransform;
    float m_Size;
    float m_zNear, m_zFar;
    Color backgroundColor;
    float fadeColorAmt = 0;

    Camera(bool editor = false);
    Camera(const Camera&);
    Camera(Camera&&);
    ~Camera();
    Camera& operator=(const Camera&);
    Camera& operator=(Camera&&);
    Matrix4x4 GetProjectionMatrix(Transform transform) const;
    Matrix4x4 GetViewMatrix(Transform transform) const;
    Vector3 Max(Transform transform) const;
    Vector3 Min(Transform transform) const;
    FrameBuffer& GetFrameBuffer();
    float GetAspectRatio();
    float SetAspectRatio(float width, float height);
    float SetViewportSize(float width);

    void Start();
    void End();
  };
}
