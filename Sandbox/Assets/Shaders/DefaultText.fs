#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D _MainTex;
uniform vec4 _Color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(_MainTex, TexCoords).r);
    color = _Color * sampled;
}