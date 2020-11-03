#pragma once

#include "DEpch.h"
#include "Shader.h"

namespace DeltaEngine
{
  class Material
  {
    std::string m_ShaderKey;
    std::unordered_map<std::string, std::string> m_Uniforms;
  public:
    Material( std::string shaderName = "Default" );
    void ChangeShader(Shader* shaderRef);
    void ChangeShader(std::string shaderName);
  };
}
