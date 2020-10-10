#include "SpriteRenderer.h"
#include "OpenGLSystem.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{
	SpriteRenderer::SpriteRenderer(Texture2D* t, Shader* s) : sprite{ t ? t->GetName() : "" },
		m_Offset{ Vector2(0, 0) }, m_Tiling{ Vector2(1, 1) }, m_FlipX{ false }, m_FlipY{ false },
		m_Shaded{ true }, m_Wireframe{ true }
	{
		shader = s;
		if (!shader)
			shader = GetEnv().pManager->Get<Shader>("Default");
		RenderModule::allRenderers.push_back(this);
	}
	SpriteRenderer::~SpriteRenderer()
	{
		DeltaEngine_CORE_INFO("Deleting Sprite Renderer");
		DeltaEngine_CORE_INFO("Sprite Renderer deleted");
	}
	void SpriteRenderer::Render(const Camera& camera)
	{
		Vector2 offset = m_Offset + sprite.GetOffset();
		Vector2 tiling = m_Tiling * sprite.GetTiling();
		Vector2 pivot = sprite.GetPivot();

		Matrix4x4 proj = camera.GetProjectionMatrix();
		Matrix4x4 view = camera.GetViewMatrix();
		Matrix4x4 model = Matrix4x4::Scale(Vector3{
			(sprite ? (sprite.GetWidth() / 200.0f) : 1) * (m_FlipX ? -1 : 1),
			(sprite ? (sprite.GetHeight() / 200.0f) : 1) * (m_FlipY ? -1 : 1), 1 })
			* transform.LocalToWorldMatrix();

		if (m_Shaded)
		{
			if (sprite)
			{
				sprite.GetTexture()->Bind(0);
			}

			shader->SetUniformMatrix4f("_M", model);
			shader->SetUniformMatrix4f("_V", view);
			shader->SetUniformMatrix4f("_P", proj);
			shader->SetUniformColor4f("_Color", color);
			shader->SetUniform1i("_MainTex", 0);
			Mesh::DrawQuad(offset, tiling, pivot);

			if (sprite)
			{
				sprite.GetTexture()->Unbind();
			}
		}
		if (m_Wireframe)
		{
			shader->SetUniformMatrix4f("_M", model);
			shader->SetUniformMatrix4f("_V", view);
			shader->SetUniformMatrix4f("_P", proj);
			Mesh::DrawQuad(true);
		}
	}
}
