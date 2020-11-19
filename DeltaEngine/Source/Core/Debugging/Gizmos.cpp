#include "Gizmos.h"
#include "Render/Shader.h"
#include "Render/Mesh.h"
#include "Render/Camera.h"
#include "Core/Math/Math.h"

namespace DeltaEngine::Gizmos
{
  Shader* gizmoShader;
  Color color = Color(0.0f, 1.0f, 0.0f, 1.0f);

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
    glClear(GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Transform t = Camera::editorCamera->transform;
    t.position.x = 0;
    t.position.y = 0;

    Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
    Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
    Matrix4x4 model = t.LocalToWorldMatrix();

    int i = 0;
    float size = Math::Abs(Camera::editorCamera->m_Size);
    float minX = (Camera::editorCamera->Min().x) - Camera::editorCamera->m_Size * Camera::editorCamera->GetAspectRatio()
      / 2;
    float minY = (Camera::editorCamera->Min().y) - Camera::editorCamera->m_Size / 2;
    float maxX = (Camera::editorCamera->Max().x) + Camera::editorCamera->m_Size * Camera::editorCamera->GetAspectRatio()
      / 2;
    float maxY = (Camera::editorCamera->Max().y) + Camera::editorCamera->m_Size / 2;

    if (size < 50)
    {
      Color col = Color(1.0f, 1.0f, 1.0f, 0.1f);
      col.a = Math::Lerp(0.0f, 0.1f, Math::Clamp01((50 - size) / 50));

      gizmoShader->SetUniformMatrix4f("_M", model);
      gizmoShader->SetUniformMatrix4f("_V", view);
      gizmoShader->SetUniformMatrix4f("_P", proj);
      gizmoShader->SetUniformColor4f("_Color", col);
      gizmoShader->SetUniform1i("_Circle", 0);

      std::vector<std::pair<Vector3, Vector3>> startEndPair;
      i = Math::RoundDown(minY);
      for (; i <= maxY; ++i)
        startEndPair.push_back({
          Vector3(minX, static_cast<float>(i), 0.0f),
          Vector3(maxX, static_cast<float>(i), 0.0f)
        });
      i = Math::RoundDown(minX);
      for (; i <= maxX; ++i)
        startEndPair.push_back({
          Vector3(static_cast<float>(i), minY, 0.0f),
          Vector3(static_cast<float>(i), maxY, 0.0f)
        });

      Mesh::DrawLines(startEndPair);
    }

    if (size > 5)
    {
      Color col = Color(1.0f, 1.0f, 1.0f, 0.1f);
      if (size < 25)
        col.a = Math::Lerp(0.1f, 0.0f, Math::Clamp01((25 - size) / 25));

      gizmoShader->SetUniformMatrix4f("_M", model);
      gizmoShader->SetUniformMatrix4f("_V", view);
      gizmoShader->SetUniformMatrix4f("_P", proj);
      gizmoShader->SetUniformColor4f("_Color", col);
      gizmoShader->SetUniform1i("_Circle", 0);

      std::vector<std::pair<Vector3, Vector3>> startEndPair;
      i = Math::RoundDown(minY);
      i -= i % 10;
      for (; i <= maxY; i += 10)
        startEndPair.push_back({
          Vector3(minX, static_cast<float>(i), 0.0f),
          Vector3(maxX, static_cast<float>(i), 0.0f)
        });
      i = Math::RoundDown(minX);
      i -= i % 10;
      for (; i <= maxX; i += 10)
        startEndPair.push_back({
          Vector3(static_cast<float>(i), minY, 0.0f),
          Vector3(static_cast<float>(i), maxY, 0.0f)
        });

      Mesh::DrawLines(startEndPair);
    }
  }

  void DrawLine(Vector3 start, Vector3 end)
  {
    glClear(GL_DEPTH_BUFFER_BIT);
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
    glClear(GL_DEPTH_BUFFER_BIT);
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
    Draw2DWireBox(Transform(position, rotation, scale), Vector2::zero());
  }

  void Draw2DWireBox(Transform transform,Vector2 col_offset)
  {
    glClear(GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    transform.position = transform.position + col_offset;
    Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
    Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
    Matrix4x4 model = transform.LocalToWorldMatrix();

    gizmoShader->SetUniformMatrix4f("_M", model);
    gizmoShader->SetUniformMatrix4f("_V", view);
    gizmoShader->SetUniformMatrix4f("_P", proj);
    gizmoShader->SetUniformColor4f("_Color", color);
    gizmoShader->SetUniform1i("_Circle", 0);

    std::vector<std::pair<Vector3, Vector3>> startEndPair;
    startEndPair.push_back({Vector3(-0.5f, 0.5f, 0.0f), Vector3(0.5f, 0.5f, 0.0f)});
    startEndPair.push_back({Vector3(0.5f, 0.5f, 0.0f), Vector3(0.5f, -0.5f, 0.0f)});
    startEndPair.push_back({Vector3(0.5f, -0.5f, 0.0f), Vector3(-0.5f, -0.5f, 0.0f)});
    startEndPair.push_back({Vector3(-0.5f, -0.5f, 0.0f), Vector3(-0.5f, 0.5f, 0.0f)});

    Mesh::DrawLines(startEndPair);
  }

  void Draw2DCircle(Vector3 position, Vector3 scale, Quaternion rotation)
  {
    Draw2DCircle(Transform(position, rotation, scale));
  }

  void Draw2DCircle(Transform transform)
  {
    glClear(GL_DEPTH_BUFFER_BIT);
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
    glClear(GL_DEPTH_BUFFER_BIT);
    Draw2DWireCircle(Transform(position, rotation, scale));
  }

  void Draw2DWireCircle(Transform transform)
  {
    glClear(GL_DEPTH_BUFFER_BIT);
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
        Vector3(cosf((theta + 1) / segments * Math::pi * 2), sinf((theta + 1) / segments * Math::pi * 2), 0.0f) / 2
      });
    }

    Mesh::DrawLines(startEndPair);
  }
}
