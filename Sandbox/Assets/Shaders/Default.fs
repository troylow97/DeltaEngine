#version 330 core

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 color;

uniform sampler2D _MainTex;
uniform vec4 _Color = vec4(1, 1, 1, 1);

//****************
// 0 - no fill
// 1 - horizontal left to right
// 2 - horizontal right to left
// 3 - vertical top to bottom
// 4 - vertical bottom to top
// 5 - radial clockwise
// 6 - radial anti-clockwise
uniform int _FillType = 0;
uniform float _FillAmount = 1;
uniform vec4 _SpriteUV = vec4(0, 0, 1, 1);

//***************
// radial properties
uniform float _RRot = 0;
uniform float _RStart = 0;
uniform float _REnd = 180;

void main()
{
	if (_FillType == 1)
	{
		if (vertexTexCoord.x > _FillAmount * (_SpriteUV.z - _SpriteUV.x) + _SpriteUV.x)
			discard;
	}
	if (_FillType == 2)
	{
		if (vertexTexCoord.x < (1 - _FillAmount) * (_SpriteUV.z - _SpriteUV.x) + _SpriteUV.x)
			discard;
	}
	if (_FillType == 3)
	{
		if (vertexTexCoord.y > _FillAmount * (_SpriteUV.w - _SpriteUV.y) + _SpriteUV.y)
			discard;
	}
	if (_FillType == 4)
	{
		if (vertexTexCoord.y < (1 - _FillAmount) * (_SpriteUV.w - _SpriteUV.y) + _SpriteUV.y)
			discard;
	}
	if (_FillType == 5)
	{
		float rotAngle = clamp(0, 360, 360 * (1 - clamp(0, 1, _FillAmount)));
		float startAngle = _RRot - _REnd;
        float endAngle = _RRot + _REnd + rotAngle;

        // check offsets
        float offset0 = clamp(0, 360, startAngle + 360);
        float offset360 = clamp(0, 360, endAngle - 360);

        // convert uv to atan coordinates
        vec2 atan2Coord = vec2(
			mix(-1, 1, (vertexTexCoord.x - _SpriteUV.x) / (_SpriteUV.z - _SpriteUV.x)),
			mix(-1, 1, (vertexTexCoord.y - _SpriteUV.y) / (_SpriteUV.w - _SpriteUV.y)));
        float atanAngle = atan(atan2Coord.y, atan2Coord.x) * 57.3; // angle in degrees

        // convert angle to 360 system
        if(atanAngle < 0) atanAngle = 360 + atanAngle;

        if(atanAngle >= startAngle && atanAngle <= endAngle) discard;
        if(atanAngle <= offset360) discard;
        if(atanAngle >= offset0) discard;
	}
	if (_FillType == 6)
	{
		float rotAngle = clamp(0, 360, 360 * (1 - clamp(0, 1, _FillAmount)));
		float startAngle = _RRot - _RStart - rotAngle;
        float endAngle = _RRot + _REnd;

        // check offsets
        float offset0 = clamp(0, 360, startAngle + 360);
        float offset360 = clamp(0, 360, endAngle - 360);

        // convert uv to atan coordinates
        vec2 atan2Coord = vec2(
			mix(-1, 1, (vertexTexCoord.x - _SpriteUV.x) / (_SpriteUV.z - _SpriteUV.x)),
			mix(-1, 1, (vertexTexCoord.y - _SpriteUV.y) / (_SpriteUV.w - _SpriteUV.y)));
        float atanAngle = atan(atan2Coord.y, atan2Coord.x) * 57.3; // angle in degrees

        // convert angle to 360 system
        if(atanAngle < 0) atanAngle = 360 + atanAngle;

        if(atanAngle >= startAngle && atanAngle <= endAngle) discard;
        if(atanAngle <= offset360) discard;
        if(atanAngle >= offset0) discard;
	}
	color = texture(_MainTex, vertexTexCoord) * vertexColor * _Color;
}
