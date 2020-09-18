#include "SpriteRenderer.h"
#include "Time.h"
#include <iostream>

namespace DeltaEngine
{
	SpriteRenderer::SpriteRenderer() : sprite{ new Texture2D("dog.png") }, shader{ new Shader() }, spriteDist{ new Texture2D("Displacements.png") }
	{
		
	}
	SpriteRenderer::~SpriteRenderer()
	{
		delete sprite;
		delete shader;
	}
	void SpriteRenderer::Render(const Camera& camera)
	{
		Matrix4x4 proj = camera.GetProjectionMatrix();
		Matrix4x4 view = camera.GetViewMatrix();
		Matrix4x4 model = Matrix4x4::Scale(Vector3{ sprite->GetWidth() / 100.0f, sprite->GetHeight() / 100.0f, 1 }) * transform.LocalToWorldMatrix();
		Matrix4x4 mvp = model * view * proj;

		static float t = 0;
		t += 0.001f;

		sprite->Bind(0);
		spriteDist->Bind(1);
		shader->SetUniformMatrix4f("_MVP", mvp);
		shader->SetUniformMatrix4f("_MP", model * proj);
		shader->SetUniformMatrix4f("_M", model);
		shader->SetUniformMatrix4f("_V", view);
		shader->SetUniformMatrix4f("_P", proj);
		shader->SetUniformColor4f("_Color", color);
		shader->SetUniform1i("_MainTex", 0);
		shader->SetUniform1i("_DistTex", 1);
		shader->SetUniform1f("_Magnitude", 0.03f);
		shader->SetUniform1f("_Time", t);
		Mesh::DrawQuad();
	}
}
