#include "Material.h"

namespace DeltaEngine
{
  Material::Material( std::string shaderName )
    : m_ShaderKey{ shaderName }
  {

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

}
