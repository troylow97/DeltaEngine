#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

out vec3 vertexPosition;
out vec4 vertexColor;
out vec2 vertexTexCoord;

void main()
{
    gl_Position = vec4(position.x * 2, -position.y * 2, 0.0, 1.0);

	vertexPosition = gl_Position.xyz;
	vertexColor = color;
	vertexTexCoord = texCoord;
}