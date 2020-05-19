#include "SpriteRenderer.h"
#include "Time.h"
#include <iostream>

namespace DeltaEngine
{
	SpriteRenderer::SpriteRenderer() : mesh{ new Mesh() }, sprite{ new Texture2D("sample.png") }, shader{ new Shader() }
	{
		color = Color::yellow();
	}
	SpriteRenderer::~SpriteRenderer()
	{
		delete mesh;
		delete sprite;
		delete shader;
	}
	void SpriteRenderer::Update()
	{
		//transform.position = Vector3(1.00f, 0.0f, 0.0f);
		transform.rotation = Quaternion::AngleAxis(45, Vector3{ 0, 0, 1 });
		transform.scale = Vector3(1000.0f, 10.0f, 1.0f);

		Matrix4x4 proj = Matrix4x4::Transpose(Matrix4x4::Ortho(-16.0f, 16.0f, -9.0f, 9.0f, -10.0f, 10.0f));
		Matrix4x4 view = Matrix4x4::Transpose(Matrix4x4::Translate(Vector3(0, 0, 0)));
		Matrix4x4 model = transform.LocalToWorldMatrix();
		Matrix4x4 mvp = model * view * proj;

		sprite->Bind(0);
		shader->SetUniformMatrix4f("_MVP", mvp);
		shader->SetUniformColor4f("_Color", color);
		shader->SetUniform1i("_MainTex", 0);
		mesh->Draw();
	}
}
