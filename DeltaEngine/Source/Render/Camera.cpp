#include "Camera.h"
#include <algorithm>
#include "Window.h"

namespace DeltaEngine
{
	std::vector<Camera*> Camera::allCameras;
	Camera* Camera::editorCamera;
	Camera::Camera(bool editor) : _size{ 5 }, _zNear{ -10 }, _zFar{ 10 }, backgroundColor{ 49 / 255.0f, 77 / 255.0f, 121 / 255.0f, 1 }, cameraIndex{ editor ? -1 : static_cast<int>(allCameras.size()) }
	{
		if (!editor)
			allCameras.push_back(this);
		else
		{
			if (!editorCamera)
				editorCamera = this;
			else
			{
				DeltaEngine_CORE_ERROR("An editor camera already exists, only one should exist at a time");
				delete this;
			}
		}
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
	Vector3 Camera::Max() const
	{
		return Vector3(_size / RenderModule::height * RenderModule::width + transform.position.x, _size + transform.position.y);
	}
	Vector3 Camera::Min() const
	{
		return Vector3(-_size / RenderModule::height * RenderModule::width + transform.position.x, -_size + transform.position.y);
	}
}

