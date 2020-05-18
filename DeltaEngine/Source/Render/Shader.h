#pragma once

#include <string>
#include <unordered_map>
#include "Math/Color.h"
#include "Math/Matrix.h"

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
	ShaderProgramSource m_filepath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_uniformLocationCache;
public:
	Shader();
	Shader(const std::string& filepath);
	Shader(const std::string& vsfilepath, const std::string& fsfilepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	unsigned int GetShaderID();
	// Set uniforms
	void SetUniform1i(const std::string& name, int i0);
	void SetUniformMatrix4f(const std::string& name, const Matrix4x4& matrix);
	void SetUniformColor4f(const std::string& name, Color& c);
	void SetUniformVector4f(const std::string& name, Vector4& v);
	void SetUniformVector4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	std::string LoadShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};
