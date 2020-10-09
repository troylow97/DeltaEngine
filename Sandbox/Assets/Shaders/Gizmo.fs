#version 330 core

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 color;

uniform vec4 _Color = vec4(1, 1, 1, 1);
uniform int _Circle = 0;

void main()
{
	color = _Color;

	if (_Circle > 0)
	{
		float dist = distance(vertexPosition, vec3(0,0,0));

		if (dist > 0.5)
			discard;

		//color.a = smoothstep(0.5, 0.495, dist) * smoothstep(0.495, 0.5, dist);
	}
}
