#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4 _M; //ModelMatrix
uniform mat4 _V; //ViewMatrix
uniform mat4 _P; //ProjectionMatrix

void main()
{
	vec4 v = vec4(vertex.xy, 0.0, 1.0); 
	vec4 v1 = _M * v;
	vec4 v2 = _V * v1;
	vec4 v3 = _P * v2;
	gl_Position = v3;

    TexCoords = vertex.zw;
}  