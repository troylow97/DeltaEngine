#version 330 core
in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;

out vec4 color;

uniform sampler2D _MainTex;
uniform vec4 _Color = vec4(1, 1, 1, 1);
uniform float _FadeAmt = 0;
uniform vec4 _FadeColor = vec4(0, 0, 0, 1);

void main()
{
  color = texture(_MainTex, vertexTexCoord) * vertexColor * _Color;
  color = mix(color, vec4(_FadeColor.r, _FadeColor.g, _FadeColor.b, color.a), _FadeAmt);
} 