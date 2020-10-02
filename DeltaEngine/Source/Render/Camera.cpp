#include "Camera.h"
#include <algorithm>
#include "Window.h"
#include "Mesh.h"
#include "OpenGLSystem.h"
#include <GL/glew.h>
#include "Core/Debugging/Gizmos.h"

namespace DeltaEngine
{
	std::vector<Camera*> Camera::allCameras;
	Camera* Camera::editorCamera;
	Camera::Camera(bool editor) :
		cameraIndex{ editor ? -1 : static_cast<int>(allCameras.size()) }, frameBuffer{},
		_size{ 5 }, _zNear{ -10 }, _zFar{ 10 },
		backgroundColor{ 49 / 255.0f, 77 / 255.0f, 121 / 255.0f, 1 },
		shader{ new Shader("Shaders/DefaultScreen") }
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
		return Matrix4x4::Rotate(transform.rotation) * Matrix4x4::Ortho(
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

	void Camera::Render()
	{
		Camera* thisCam = this;
		frameBuffer.Resize(RenderModule::width, RenderModule::height);

		frameBuffer.Bind();

		glClearColor(
			backgroundColor.r,
			backgroundColor.g,
			backgroundColor.b,
			backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		std::for_each(RenderModule::allRenderers.begin(), RenderModule::allRenderers.end(), [thisCam](Renderer* s)
			{ s->Render(*thisCam); });

		// Call all OnDrawGizmos() here

		if (this == editorCamera)
		{
			Gizmos::DrawWorldGrid();
		}

		frameBuffer.Unbind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindTexture(GL_TEXTURE_2D, frameBuffer.GetColorAttachment());	// use the color attachment texture as the texture of the quad plane
		shader->SetUniform1i("_MainTex", 0);

		Mesh::DrawQuad();
	}
}

