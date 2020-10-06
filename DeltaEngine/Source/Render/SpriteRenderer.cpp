#include "SpriteRenderer.h"
#include "OpenGLSystem.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
	SpriteRenderer::SpriteRenderer(Texture2D* t, Shader* s) : sprite{ t->GetName() },
		m_Offset{ Vector2(0, 0) }, m_Tiling{ Vector2(1, 1) }
	{
		shader = s;
		RenderModule::allRenderers.push_back(this);
	}
	SpriteRenderer::~SpriteRenderer()
	{
		DeltaEngine_CORE_INFO("Deleting Sprite Renderer");
		delete shader;
		DeltaEngine_CORE_INFO("Sprite Renderer deleted");
	}
	void SpriteRenderer::Render(const Camera& camera)
	{
		//static float f = 0;
		//f += DeltaTime();
		//sprite.m_Index = (unsigned int)(f) % sprite.GetTexture()->textureInfo.size();

		Vector2 offset = m_Offset + sprite.GetOffset();
		Vector2 tiling = m_Tiling * sprite.GetTiling();
		Vector2 pivot = sprite.GetPivot();

		Matrix4x4 proj = camera.GetProjectionMatrix();
		Matrix4x4 view = camera.GetViewMatrix();
		Matrix4x4 model = Matrix4x4::Scale(Vector3{
			sprite ? (sprite.GetWidth() / 200.0f) : 1,
			sprite ? (sprite.GetHeight() / 200.0f) : 1, 1 })
			* transform.LocalToWorldMatrix();

		if (sprite)
		{
			sprite.GetTexture()->Bind(0);
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
		Mesh::DrawQuad(offset, tiling, pivot);
	}
}
