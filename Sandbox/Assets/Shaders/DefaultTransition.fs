#version 330 core
in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;

out vec4 color;

uniform sampler2D _MainTex;
uniform sampler2D _DispTex;
uniform vec4 _Color = vec4(1, 1, 1, 1);
uniform float _Cutoff = 0;
uniform float _Blur = 0;
uniform vec4 _CutoffColor = vec4(0, 0, 0, 1);

void main()
{
  vec4 disp = texture(_DispTex, vertexTexCoord);
  color = texture(_MainTex, vertexTexCoord) * vertexColor * _Color;
  if (disp.x < _Cutoff)
  {
    color = _CutoffColor;
  }
  if (disp.x < _Cutoff + _Blur)
  {
    color = mix(color, _CutoffColor, (_Cutoff + _Blur - disp.x) / _Blur);
  }
} 