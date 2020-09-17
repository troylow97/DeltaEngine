#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

out vec3 vertexPosition;
out vec4 vertexColor;
out vec2 vertexTexCoord;

uniform mat4 _MVP;
uniform mat4 _M;

void main()
{
	gl_Position = _MVP * vec4(position, 1.0);
	vertexPosition = (_MVP * vec4(position, 1.0)).xyz;
	vertexColor = color;
	vertexTexCoord = texCoord;
}
