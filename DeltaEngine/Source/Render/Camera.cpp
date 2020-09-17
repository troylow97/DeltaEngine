#include "Camera.h"
#include <algorithm>
#include "Camera.h"
#include "Window.h"

namespace DeltaEngine
{
	std::vector<Camera*> Camera::allCameras;
	Camera::Camera() : _size{ 1 }, _zNear{ -10 }, _zFar{ 10 }, cameraIndex{ allCameras.size() }
	{
		allCameras.push_back(this);
	}
	Camera::~Camera()
	{
		allCameras.erase(allCameras.begin() + cameraIndex);

		for (size_t i = 0; i < allCameras.size(); ++i)
			allCameras[i]->cameraIndex = i;
	}
	Matrix4x4 Camera::GetProjectionMatrix() const
	{
		return Matrix4x4::Ortho(
			-_size / RenderModule::height * RenderModule::width,
			_size / RenderModule::height * RenderModule::width,
			-_size, _size, _zNear, _zFar);
	}
	Matrix4x4 Camera::GetViewMatrix() const
	{
		return Matrix4x4::Transpose(Matrix4x4::Translate(-transform.position));;
	}
}

