#include "SpriteRenderer.h"
#include "Time.h"
#include <iostream>

namespace DeltaEngine
{
	SpriteRenderer::SpriteRenderer() : mesh{ new Mesh() }, sprite{ new Texture2D("dog.png") }, shader{ new Shader() }, spriteDist{ new Texture2D("Displacements.png") }
	{
		
	}
	SpriteRenderer::~SpriteRenderer()
	{
		delete mesh;
		delete sprite;
		delete shader;
	}
	void SpriteRenderer::Render(const Camera& camera)
	{
		//transform.position = Vector3(0.0f, 0.0f, 0.0f);
		//transform.rotation = Quaternion::AngleAxis(0, Vector3(0, 0, 1));
		//transform.scale = Vector3(1.0f, 1.0f, 1.0f);

		//Matrix4x4 proj = Matrix4x4::Transpose(Matrix4x4::Ortho(-4.0f, 4.0f, -3.0f, 3.0f, -10.0f, 10.0f));
		Matrix4x4 proj = camera.GetProjectionMatrix();
		//Matrix4x4 view = Matrix4x4::Transpose(Matrix4x4::Translate(Vector3(0, 0, 0)));
		Matrix4x4 view = camera.GetViewMatrix();
		Matrix4x4 model = transform.LocalToWorldMatrix();
		Matrix4x4 mvp = model * view * proj;

		static float t = 0;
		t += 0.001f;

		sprite->Bind(0);
		spriteDist->Bind(1);
		shader->SetUniformMatrix4f("_MVP", mvp);
		shader->SetUniformMatrix4f("_M", model);
		shader->SetUniformColor4f("_Color", color);
		shader->SetUniform1i("_MainTex", 0);
		shader->SetUniform1i("_DistTex", 1);
		shader->SetUniform1f("_Magnitude", 0.03f);
		shader->SetUniform1f("_Time", t);
		mesh->Draw();
	}
}
