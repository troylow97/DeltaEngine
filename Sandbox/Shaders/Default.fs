#version 330 core

in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 color;

uniform sampler2D _MainTex;
uniform vec4 _Color;

void main()
{
	color = vertexColor * _Color;

	//color = texture(_MainTex, vertexTexCoord) * vertexColor * _Color;
}
