#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
// inst
layout (location = 3) in vec4 p0;
layout (location = 4) in vec4 p1;
layout (location = 5) in vec4 p2;
layout (location = 6) in vec4 col;

out vec3 vertexPosition;
out vec4 vertexColor;
out vec2 vertexTexCoord;

uniform mat4 _M; //ModelMatrix
uniform mat4 _V; //ViewMatrix
uniform mat4 _P; //ProjectionMatrix

void main()
{
  mat4 pMat;
  
  pMat[0] = vec4(p0.x, p1.x, p2.x, 0);
  pMat[1] = vec4(p0.y, p1.y, p2.y, 0);
  pMat[2] = vec4(p0.z, p1.z, p2.z, 0);
  pMat[3] = vec4(p0.w, p1.w, p2.w, 1);
  
  //pMat[0] = vec4(1, 0, 0, 0);
  //pMat[1] = vec4(0, 1, 0, 0);
  //pMat[2] = vec4(0, 0, 1, 0);
  //pMat[3] = vec4(gl_InstanceID, 0, 0, 1);
  
  vec4 v = vec4(position, 1.0); 
  vec4 v1 = _M * (pMat * v);
  vec4 v2 = _V * v1;
  vec4 v3 = _P * v2;
  gl_Position = v3;
  
  vertexPosition = v1.xyz;
  vertexColor = color * col;
  vertexTexCoord = texCoord;
}
