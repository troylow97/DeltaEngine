#include "ParticleSystem.h"

namespace DeltaEngine
{
	void ParticleSystem::Render()
	{
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);

		//float vertices[] = {
		//	 -0.5f, -0.5f, 0.0f,
		//	  0.5f, -0.5f, 0.0f,
		//	  0.5f,  0.5f, 0.0f,
		//	 -0.5f,  0.5f, 0.0f
		//};

		//glCreateVertexArrays(1, &m_QuadVA);
		//glBindVertexArray(m_QuadVA);

		//GLuint quadVB, quadIB;
		//glCreateBuffers(1, &quadVB);
		//glBindBuffer(GL_ARRAY_BUFFER, quadVB);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//glEnableVertexArrayAttrib(quadVB, 0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		//uint32_t indices[] = {
		//	0, 1, 2, 2, 3, 0
		//};

		//glCreateBuffers(1, &quadIB);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//m_ParticleShader = std::unique_ptr<GLCore::Utils::Shader>(GLCore::Utils::Shader::FromGLSLTextFiles("assets/shader.glsl.vert", "assets/shader.glsl.frag"));
		//m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_ViewProj");
		//m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_Transform");
		//m_ParticleShaderColor = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_Color");
	}
}
