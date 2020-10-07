#include "Gizmos.h"
#include "Render/Shader.h"
#include "Render/Mesh.h"
#include "Render/Camera.h"
#include "Core/Math/Math.h"

namespace DeltaEngine::Gizmos
{
	Shader* gizmoShader;
	Color color = Color(0.0f, 1.0f, 0.0f, 0.5f);

	void Init()
	{
		gizmoShader = new Shader("Shaders/Gizmo");
	}
	void Exit()
	{
		delete gizmoShader;
	}

	void SetColor(Color c)
	{
		color = c;
	}

	void DrawWorldGrid()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
		Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
		Matrix4x4 model = Transform().LocalToWorldMatrix();
		
		int i = 0;

		if (Camera::editorCamera->_size < 50)
		{
			Color col = Color(1.0f, 1.0f, 1.0f, 0.1f);
			col.a = Math::Lerp(0.0f, 0.1f, Math::Clamp01((50 - Camera::editorCamera->_size) / 50));

			gizmoShader->SetUniformMatrix4f("_M", model);
			gizmoShader->SetUniformMatrix4f("_V", view);
			gizmoShader->SetUniformMatrix4f("_P", proj);
			gizmoShader->SetUniformColor4f("_Color", col);
			gizmoShader->SetUniform1i("_Circle", 0);

			std::vector<std::pair<Vector3, Vector3>> startEndPair;
			i = Math::RoundDown(Camera::editorCamera->Min().y);
			for (; i <= Camera::editorCamera->Max().y; ++i)
				startEndPair.push_back({
					Vector3(Camera::editorCamera->Min().x, static_cast<float>(i), 0.0f),
					Vector3(Camera::editorCamera->Max().x, static_cast<float>(i), 0.0f) });
			i = Math::RoundDown(Camera::editorCamera->Min().x);
			for (; i <= Camera::editorCamera->Max().x; ++i)
				startEndPair.push_back({
					Vector3(static_cast<float>(i), Camera::editorCamera->Min().y, 0.0f),
					Vector3(static_cast<float>(i), Camera::editorCamera->Max().y, 0.0f) });

			Mesh::DrawLines(startEndPair);
		}

		if (Camera::editorCamera->_size > 5)
		{
			Color col = Color(1.0f, 1.0f, 1.0f, 0.1f);
			if (Camera::editorCamera->_size < 25)
				col.a = Math::Lerp(0.1f, 0.0f, Math::Clamp01((25 - Camera::editorCamera->_size) / 25));

			gizmoShader->SetUniformMatrix4f("_M", model);
			gizmoShader->SetUniformMatrix4f("_V", view);
			gizmoShader->SetUniformMatrix4f("_P", proj);
			gizmoShader->SetUniformColor4f("_Color", col);
			gizmoShader->SetUniform1i("_Circle", 0);

			std::vector<std::pair<Vector3, Vector3>> startEndPair;
			i = Math::RoundDown(Camera::editorCamera->Min().y); i -= i % 10;
			for (; i <= Camera::editorCamera->Max().y; i += 10)
				startEndPair.push_back({
					Vector3(Camera::editorCamera->Min().x, static_cast<float>(i), 0.0f),
					Vector3(Camera::editorCamera->Max().x, static_cast<float>(i), 0.0f) });
			i = Math::RoundDown(Camera::editorCamera->Min().x); i -= i % 10;
			for (; i <= Camera::editorCamera->Max().x; i += 10)
				startEndPair.push_back({
					Vector3(static_cast<float>(i), Camera::editorCamera->Min().y, 0.0f),
					Vector3(static_cast<float>(i), Camera::editorCamera->Max().y, 0.0f) });

			Mesh::DrawLines(startEndPair);
		}
	}

	void DrawLine(Vector3 start, Vector3 end)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
		Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
		Matrix4x4 model = Transform().LocalToWorldMatrix();

		gizmoShader->SetUniformMatrix4f("_M", model);
		gizmoShader->SetUniformMatrix4f("_V", view);
		gizmoShader->SetUniformMatrix4f("_P", proj);
		gizmoShader->SetUniformColor4f("_Color", color);
		gizmoShader->SetUniform1i("_Circle", 0);
		
		Mesh::DrawLine(start, end);
	}

	void Draw2DBox(Vector3 position, Vector3 scale, Quaternion rotation)
	{
		Draw2DBox(Transform(position, rotation, scale));
	}
	void Draw2DBox(Transform transform)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
		Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
		Matrix4x4 model = transform.LocalToWorldMatrix();

		gizmoShader->SetUniformMatrix4f("_M", model);
		gizmoShader->SetUniformMatrix4f("_V", view);
		gizmoShader->SetUniformMatrix4f("_P", proj);
		gizmoShader->SetUniformColor4f("_Color", color);
		gizmoShader->SetUniform1i("_Circle", 0);

		Mesh::DrawQuad();
	}

	void Draw2DWireBox(Vector3 position, Vector3 scale, Quaternion rotation)
	{
		Draw2DWireBox(Transform(position, rotation, scale));
	}
	void Draw2DWireBox(Transform transform)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
		Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
		Matrix4x4 model = transform.LocalToWorldMatrix();

		gizmoShader->SetUniformMatrix4f("_M", model);
		gizmoShader->SetUniformMatrix4f("_V", view);
		gizmoShader->SetUniformMatrix4f("_P", proj);
		gizmoShader->SetUniformColor4f("_Color", color);
		gizmoShader->SetUniform1i("_Circle", 0);

		std::vector<std::pair<Vector3, Vector3>> startEndPair;
		startEndPair.push_back({ Vector3(-0.5f,  0.5f, 0.0f), Vector3( 0.5f,  0.5f, 0.0f) });
		startEndPair.push_back({ Vector3( 0.5f,  0.5f, 0.0f), Vector3( 0.5f, -0.5f, 0.0f) });
		startEndPair.push_back({ Vector3( 0.5f, -0.5f, 0.0f), Vector3(-0.5f, -0.5f, 0.0f) });
		startEndPair.push_back({ Vector3(-0.5f, -0.5f, 0.0f), Vector3(-0.5f,  0.5f, 0.0f) });

		Mesh::DrawLines(startEndPair);
	}

	void Draw2DCircle(Vector3 position, Vector3 scale, Quaternion rotation)
	{
		Draw2DCircle(Transform(position, rotation, scale));
	}
	void Draw2DCircle(Transform transform)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
		Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
		Matrix4x4 model = transform.LocalToWorldMatrix();

		gizmoShader->SetUniformMatrix4f("_M", model);
		gizmoShader->SetUniformMatrix4f("_V", view);
		gizmoShader->SetUniformMatrix4f("_P", proj);
		gizmoShader->SetUniformColor4f("_Color", color);
		gizmoShader->SetUniform1i("_Circle", 1);

		Mesh::DrawQuad();
	}

	void Draw2DWireCircle(Vector3 position, Vector3 scale, Quaternion rotation)
	{
		Draw2DWireCircle(Transform(position, rotation, scale));
	}
	void Draw2DWireCircle(Transform transform)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
		Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
		Matrix4x4 model = transform.LocalToWorldMatrix();

		gizmoShader->SetUniformMatrix4f("_M", model);
		gizmoShader->SetUniformMatrix4f("_V", view);
		gizmoShader->SetUniformMatrix4f("_P", proj);
		gizmoShader->SetUniformColor4f("_Color", color);
		gizmoShader->SetUniform1i("_Circle", 0);

		std::vector<std::pair<Vector3, Vector3>> startEndPair;

		int segments = 24;
		for (float theta = 0; theta < segments; ++theta)
		{
			startEndPair.push_back({
				Vector3(cosf(theta / segments * Math::pi * 2), sinf(theta / segments * Math::pi * 2), 0.0f) / 2,
				Vector3(cosf((theta + 1) / segments * Math::pi * 2), sinf((theta + 1) / segments * Math::pi * 2), 0.0f) / 2 });
		}

		Mesh::DrawLines(startEndPair);
	}
}
