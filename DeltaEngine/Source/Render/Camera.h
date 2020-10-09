#pragma once

#include "DEpch.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "ECS/Components/Transform.h"
#include "Core/Math/Color.h"

namespace DeltaEngine
{
	class Camera
	{
		int cameraIndex;
		FrameBuffer frameBuffer;
	public:
		static std::vector<Camera*> allCameras;
		// camera for editor mode only
		static Camera* editorCamera;
		float _size;
		float _zNear, _zFar;
		Color backgroundColor;
		Shader* shader;

		Transform transform;
		Camera(bool editor = false);
		~Camera();
		Matrix4x4 GetProjectionMatrix() const;
		Matrix4x4 GetViewMatrix() const;
		Vector3 Max() const;
		Vector3 Min() const;

		void Start();
		void End();
		void Render();
	};
}
