#version 330 core

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 color;

uniform sampler2D _MainTex;
uniform vec4 _Color;

void main()
{
	color = texture(_MainTex, vertexTexCoord) * vertexColor * _Color;
	float dist = distance(vertexPosition, vec3(0,0,0));

	if (dist >= 0.2 && dist <= 0.4)
		color = mix(vec4(1,0,0,1), texture(_MainTex, vertexTexCoord) * vertexColor * _Color, (dist - 0.2) / 0.2);
	else if (dist >= 0.4)
		color = texture(_MainTex, vertexTexCoord) * vertexColor * _Color;
	else
		color = vec4(1,0,0,1);
}
