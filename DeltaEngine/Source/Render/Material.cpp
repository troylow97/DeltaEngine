#include "Material.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{
  Material::Material( std::string shaderName )
    : m_ShaderKey{ shaderName }
  {
  }

  Material& Material::operator=(const Material& mat)
  {
    m_ShaderKey = std::string( mat.m_ShaderKey );
    return *this;
  }


  void Material::ChangeShader(Shader* shader)
  {
    if (shader)
      m_ShaderKey = shader->GetName();
  }
  void Material::ChangeShader(std::string shaderName)
  {
    m_ShaderKey = shaderName;
  }
  void Material::SetUniform1i( std::string name, int i0)
  {
    GetEnv().pManager->Get<Shader>(m_ShaderKey)->SetUniform1i(name, i0);
  }
  void Material::SetUniform1f( std::string name, float f0)
  {
    GetEnv().pManager->Get<Shader>(m_ShaderKey)->SetUniform1f(name, f0);
  }
  void Material::SetUniformMatrix4f( std::string name, Matrix4x4 matrix)
  {
    GetEnv().pManager->Get<Shader>(m_ShaderKey)->SetUniformMatrix4f(name, matrix);
  }
  void Material::SetUniformColor4f( std::string name, Color c)
  {
    GetEnv().pManager->Get<Shader>(m_ShaderKey)->SetUniformColor4f(name, c);
  }
  void Material::SetUniformVector3f( std::string name, Vector3 v)
  {
    GetEnv().pManager->Get<Shader>(m_ShaderKey)->SetUniformVector3f(name, v);
  }
  void Material::SetUniformVector4f( std::string name, Vector4 v)
  {
    GetEnv().pManager->Get<Shader>(m_ShaderKey)->SetUniformVector4f(name, v);
  }
  void Material::SetUniformVector4f( std::string name, float v0, float v1, float v2, float v3)
  {
    GetEnv().pManager->Get<Shader>(m_ShaderKey)->SetUniformVector4f(name, v0, v1, v2, v3);
  }
}
