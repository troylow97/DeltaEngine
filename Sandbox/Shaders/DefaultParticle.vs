#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 p0;
layout (location = 4) in vec4 p1;
layout (location = 5) in vec4 p2;
layout (location = 6) in vec4 p3;

out vec3 vertexPosition;
out vec4 vertexColor;
out vec2 vertexTexCoord;

uniform vec3 offsets[100];

uniform mat4 _M; //ModelMatrix
uniform mat4 _V; //ViewMatrix
uniform mat4 _P; //ProjectionMatrix

void main()
{
	mat4 pMat;
	
	pMat[0] = vec4(p0.x, p1.x, p2.x, p3.x);
	pMat[1] = vec4(p0.y, p1.y, p2.y, p3.y);
	pMat[2] = vec4(p0.z, p1.z, p2.z, p3.z);
	pMat[3] = vec4(p0.w, p1.w, p2.w, p3.w);

	//pMat[0] = p0;
	//pMat[1] = p1;
	//pMat[2] = p2;
	//pMat[3] = p3;

    vec3 offset = offsets[gl_InstanceID];

	vec4 v = vec4(position, 1.0); 
	vec4 v1 = _M * (pMat * v);
	vec4 v2 = _V * v1;
	vec4 v3 = _P * v2;
	gl_Position = v3;

	vertexPosition = v1.xyz;
    vertexColor = color;
	vertexTexCoord = texCoord;
}
