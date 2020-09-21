#include "Shader.h"
#include "ErrorCheck.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Core/Logger/Log.h"

namespace DeltaEngine
{
	Shader::Shader()
		: m_filepath{ "Shaders/Default.vs", "Shaders/Default.fs" }, m_RendererID{ 0 }
	{
		m_RendererID = CreateShader(LoadShader("Shaders/Default.vs"), LoadShader("Shaders/Default.fs"));
	}
	Shader::Shader(const std::string& filepath)
		: m_filepath{ filepath + ".vs", filepath + ".fs" }, m_RendererID{ 0 }
	{
		m_RendererID = CreateShader(LoadShader(filepath + ".vs"), LoadShader(filepath + ".fs"));
	}
	Shader::Shader(const std::string& vsfilepath, const std::string& fsfilepath)
		: m_filepath{ vsfilepath + ".vs", fsfilepath + ".fs" }, m_RendererID{ 0 }
	{
		m_RendererID = CreateShader(LoadShader(vsfilepath + ".vs"), LoadShader(fsfilepath + ".fs"));
	}
	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_RendererID));
	}

	void Shader::Bind() const
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLCall(glUseProgram(m_RendererID));
	}

	void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	std::string Shader::LoadShader(const std::string& filepath)
	{
		std::ifstream file;
		DeltaEngine_CORE_TRACE("Loading shader \"{}\"...", filepath.c_str());
		file.open((filepath).c_str());

		std::string output;
		std::string line;

		if (file.is_open())
		{
			while (file.good())
			{
				getline(file, line);
				output.append(line + "\n");
			}
			DeltaEngine_CORE_TRACE("Shader \"{}\" was loaded successfully.", filepath.c_str());
		}
		else
		{
			DeltaEngine_CORE_ERROR("Unable to load shader \"{}\"", filepath.c_str());
		}

		return output;
	}
	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		//error handling
		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (!result)
		{
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = new char[length];
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			DeltaEngine_CORE_ERROR("Failed to compile {} shader!", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
			DeltaEngine_CORE_ERROR("{}", message);
			GLCall(glDeleteShader(id));
			return 0;
		}

		return id;
	}
	unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		if (!vs)
			vs = CompileShader(GL_VERTEX_SHADER, LoadShader("Shaders/ErrorShader.vs"));
		if (!fs)
			fs = CompileShader(GL_FRAGMENT_SHADER, LoadShader("Shaders/ErrorShader.fs"));

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}
	unsigned int Shader::GetShaderID()
	{
		return m_RendererID;
	}

	void Shader::SetUniform1i(const std::string& name, int i0)
	{
		Bind();
		glUniform1i(GetUniformLocation(name), i0);
	}
	void Shader::SetUniformMatrix4f(const std::string& name, const Matrix4x4& matrix)
	{
		Bind();
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix.m);
	}
	void Shader::SetUniformColor4f(const std::string& name, Color& c)
	{
		Bind();
		GLCall(glUniform4f(GetUniformLocation(name), c.r, c.g, c.b, c.a));
	}
	void Shader::SetUniformVector4f(const std::string& name, Vector4& v)
	{
		Bind();
		GLCall(glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w));
	}
	void Shader::SetUniformVector4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		Bind();
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}
	int Shader::GetUniformLocation(const std::string& name)
	{
		if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
			return m_uniformLocationCache[name];

		GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
		if (location == -1)
			DeltaEngine_CORE_WARN("Warning: uniform '{}' doesn't exist!", name);
		m_uniformLocationCache[name] = location;
		return location;
	}
}
