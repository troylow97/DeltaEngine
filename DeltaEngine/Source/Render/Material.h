#pragma once

#include "DEpch.h"
#include "Shader.h"

namespace DeltaEngine
{
  struct Material
  {

  public:

    std::string m_ShaderKey;
    //std::unordered_map<std::string, std::string> m_Uniforms;

    Material( std::string shaderName = "Default" );
    void ChangeShader(Shader* shaderRef);
    void ChangeShader(std::string shaderName);
    // Set uniforms
    void SetUniform1i(const std::string& name, int i0);
    void SetUniform1f(const std::string& name, float f0);
    void SetUniformMatrix4f(const std::string& name, const Matrix4x4& matrix);
    void SetUniformColor4f(const std::string& name, Color& c);
    void SetUniformVector3f(const std::string& name, Vector3& v);
    void SetUniformVector4f(const std::string& name, Vector4& v);
    void SetUniformVector4f(const std::string& name, float v0, float v1, float v2, float v3);
  };
}
