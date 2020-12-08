/**********************************************************************************
* \file   Material.h
* \brief  The file contains interface of Material class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "DEpch.h"
#include "Shader.h"

namespace DeltaEngine
{
  struct Material
  {
    std::string m_ShaderKey;
    //std::unordered_map<std::string, std::string> m_Uniforms;

    Material(std::string shaderName = "Default");
    //Material &operator=(const Material& mat);

    void ChangeShader(Shader* shaderRef);
    void ChangeShader(std::string shaderName);
    // Set uniforms
    void SetUniform1i(std::string name, int i0);
    void SetUniform1f(std::string name, float f0);
    void SetUniformMatrix4f(std::string name, Matrix4x4 matrix);
    void SetUniformColor4f(std::string name, Color c);
    void SetUniformVector3f(std::string name, Vector3 v);
    void SetUniformVector4f(std::string name, Vector4 v);
    void SetUniformVector4f(std::string name, float v0, float v1, float v2, float v3);
  };
}
