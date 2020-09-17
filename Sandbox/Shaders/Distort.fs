#version 330 core

in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 color;

uniform sampler2D _DistTex;
uniform sampler2D _MainTex;
uniform vec4 _Color;
uniform float _Magnitude;
uniform float _Time;

void main()
{
	vec2 disp = texture(_DistTex, vertexTexCoord + vec2(cos(_Time) / 5,sin(_Time) / 5)).xy;
	disp = ((disp * 2) - 1) * _Magnitude;

	color = texture(_MainTex, vertexTexCoord + disp) * vertexColor * _Color;
}
