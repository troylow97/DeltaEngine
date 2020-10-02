#include "TextRenderer.h"
#include "OpenGLSystem.h"
#include "Mesh.h"
#include <GL/glew.h>

namespace DeltaEngine
{
    Mesh* textMesh;

    TextRenderer::TextRenderer(Font* f, Shader* s) :
        font{ f }, text{"Nomasaur - Metamorphosis"}
	{
        shader = s;
        RenderModule::allRenderers.push_back(this);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}
	void TextRenderer::Render(const Camera& camera)
	{
        Matrix4x4 proj = camera.GetProjectionMatrix();
        Matrix4x4 view = camera.GetViewMatrix();
        Matrix4x4 model = transform.LocalToWorldMatrix();

        // activate corresponding render state	
        shader->Bind();
        shader->SetUniformMatrix4f("_M", model);
        shader->SetUniformMatrix4f("_V", view);
        shader->SetUniformMatrix4f("_P", proj);
        shader->SetUniformColor4f("_Color", color);
        shader->SetUniform1i("_MainTex", 0);

        float x = transform.position.x;
        float y = transform.position.y;

        float scale = 0.01f;

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            CharacterInfo ch = font->characterInfo()[*c];

            float xpos = x + ch.bearing.x * transform.scale.x * scale;
            float ypos = y - (ch.size.y - ch.bearing.y) * transform.scale.y * scale;

            float w = ch.size.x * transform.scale.x * scale;
            float h = ch.size.y * transform.scale.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h, 0.0f, 0.0f },
                { xpos,     ypos,     0.0f, 1.0f },
                { xpos + w, ypos,     1.0f, 1.0f },

                { xpos,     ypos + h, 0.0f, 0.0f },
                { xpos + w, ypos,     1.0f, 1.0f },
                { xpos + w, ypos + h, 1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * transform.scale.x * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
	}
}
