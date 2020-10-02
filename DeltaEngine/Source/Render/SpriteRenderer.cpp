#include "SpriteRenderer.h"
#include "OpenGLSystem.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
	SpriteRenderer::SpriteRenderer(std::string textureName) : sprite{ new Texture2D(textureName) },
		offset{ Vector2() }, tiling{ Vector2(1,1) }
	{
		shader = new Shader();
		RenderModule::allRenderers.push_back(this);
	}
	SpriteRenderer::~SpriteRenderer()
	{
		DeltaEngine_CORE_INFO("Deleting Sprite Renderer");
		delete sprite;
		delete shader;
		DeltaEngine_CORE_INFO("Sprite Renderer deleted");
	}
	void SpriteRenderer::Render(const Camera& camera)
	{
		Matrix4x4 proj = camera.GetProjectionMatrix();
		Matrix4x4 view = camera.GetViewMatrix();
		Matrix4x4 model = Matrix4x4::Scale(Vector3{
			sprite ? (sprite->GetWidth() / 100.0f * tiling.x) : 1,
			sprite ? (sprite->GetHeight() / 100.0f * tiling.y) : 1, 1 })
			* transform.LocalToWorldMatrix();

		static float t = 0;
		t += 0.001f;

		if (sprite)
		{
			sprite->Bind(0);
		}
		shader->SetUniformMatrix4f("_M", model);
		shader->SetUniformMatrix4f("_V", view);
		shader->SetUniformMatrix4f("_P", proj);
		shader->SetUniformColor4f("_Color", color);
		shader->SetUniform1i("_MainTex", 0);
		//shader->SetUniform1i("_DistTex", 1);
		//shader->SetUniform1f("_Magnitude", 0.03f);
		//shader->SetUniform1f("_Time", t);
		//shader->SetUniform1f("_Radius", 0.4f);
		//shader->SetUniformVector3f("_Center", Vector3(0,0,0));
		Mesh::DrawQuad(offset.x, offset.y, tiling.x, tiling.y);
	}
}
