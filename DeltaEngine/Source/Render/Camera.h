/**********************************************************************************
* \file   Camera.h
* \brief  The file contains interface of Camera class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
        float aspectRatio, viewportSize, camWidth;
    public:
        static std::vector<Camera*> allCameras;
        // camera for editor mode only
        static FrameBuffer* finalFrameBuffer;
        static Camera* editorCamera;
        static Transform editorCameraTransform;
        static float fixedAspect;
        float m_Size, m_FieldOfView;
        float m_zNear, m_zFar;
        Color backgroundColor;
        float fadeAmt = 0;
        float blurAmt = 0.25f;
        Vector2 shakeDisp;
        float shakeMagnitude = 0, shakeFade = 10;
#pragma warning(disable:4251)
        std::string transitionTexKey;
#pragma warning(default:4251)

        bool perspective;

        Camera(bool editor = false);
        Camera(const Camera&);
        Camera(Camera&&);
        ~Camera();
        Camera& operator=(const Camera&);
        Camera& operator=(Camera&&);
        Matrix4x4 GetOrthographicMatrix(Transform transform) const;
        Matrix4x4 GetPerspectiveMatrix(Transform transform) const;
        Matrix4x4 GetProjectionMatrix(Transform transform);
        Matrix4x4 GetViewMatrix(Transform transform) const;
        Vector3 Max(Transform transform) const;
        Vector3 Min(Transform transform) const;
        FrameBuffer& GetFrameBuffer();
        float GetAspectRatio();
        float SetAspectRatio(float width, float height);
        static float GetFixedAspectRatio();
        static float SetFixedAspectRatio(float width, float height);
        float GetViewportSize();
        float SetViewportSize(float width);
        float GetTrueViewportSize();

        void Start();
        void End();

        void Shake(float magnitude = 1.f, float fade = 10.f);
    };
}
