#pragma once

#include "DEpch.h"
#include "Core/Math/Transform.h"

namespace DeltaEngine
{
	class Camera
	{
	public:
		static std::vector<Camera*> allCameras;
		// camera for editor mode only
		static Camera* editorCamera;
		float _size;
		float _zNear, _zFar;

		Transform transform;
		Camera(bool editor = false);
		~Camera();
		Matrix4x4 GetProjectionMatrix() const;
		Matrix4x4 GetViewMatrix() const;
	private:
		int cameraIndex;
	};
}
