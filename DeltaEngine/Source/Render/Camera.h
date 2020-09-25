#pragma once

#include "DEpch.h"
#include "Core/Math/Transform.h"

namespace DeltaEngine
{
	class Camera
	{
	public:
		static std::vector<Camera*> allCameras;
		float _size;
		float _zNear, _zFar;

		Transform transform;
		Camera();
		~Camera();
		Matrix4x4 GetProjectionMatrix() const;
		Matrix4x4 GetViewMatrix() const;
	private:
		size_t cameraIndex;
	};
}
