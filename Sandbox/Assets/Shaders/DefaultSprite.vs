#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 m0;
layout (location = 4) in vec4 m1;
layout (location = 5) in vec4 m2;
layout (location = 6) in float texSlot;
// fill options
layout (location = 7) in float fillType;
layout (location = 8) in float _FillAmount;
layout (location = 9) in vec4 _SpriteUV;
layout (location = 10) in float _RRot;
layout (location = 11) in float _RStart;
layout (location = 12) in float _REnd;

out vec3 vertexPosition;
out vec4 vertexColor;
out vec2 vertexTexCoord;

uniform mat4 _M; //ModelMatrix
uniform mat4 _V; //ViewMatrix
uniform mat4 _P; //ProjectionMatrix

void main()
{
  vec4 v = vec4(position, 1.0);
  vec4 v1 = _M * v;
  vec4 v2 = _V * v1;
  vec4 v3 = _P * v2;
  gl_Position = v3;
  
  vertexPosition = v1.xyz;
  vertexColor = color;
  vertexTexCoord = texCoord;
}
