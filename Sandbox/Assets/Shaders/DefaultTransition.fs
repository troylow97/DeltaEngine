#version 330 core
in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;

out vec4 color;

uniform sampler2D _MainTex;
uniform sampler2D _NextTex;
uniform sampler2D _DispTex;
uniform vec4 _Color = vec4(1, 1, 1, 1);
uniform float _Cutoff = 0;
uniform float _Blur = 0.1;

void main()
{
  vec4 disp = texture(_DispTex, vertexTexCoord);
  if (disp.x < _Cutoff)
  {
    color = texture(_NextTex, vertexTexCoord);
  }
  else if (disp.x < _Cutoff + _Blur)
  {
    color = mix(texture(_MainTex, vertexTexCoord), texture(_NextTex, vertexTexCoord), (_Cutoff + _Blur - disp.x) / _Blur);
  }
  else
  {
    color = texture(_MainTex, vertexTexCoord);
  }
} 