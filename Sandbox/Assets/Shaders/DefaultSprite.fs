#version 330 core

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;

//****************
// 0 - no fill
// 1 - horizontal left to right
// 2 - horizontal right to left
// 3 - vertical top to bottom
// 4 - vertical bottom to top
// 5 - radial clockwise
// 6 - radial anti-clockwise

in float iFillType;
in float iFillAmount;
in vec4 iSpriteUV;

//***************
// radial properties

in float iRRot;
in float iRStart;
in float iREnd;

out vec4 color;

uniform sampler2D _MainTex;
uniform vec4 _Color = vec4(1, 1, 1, 1);

void main()
{
  if (iFillType > 0)
  {
    if (iFillType < 2)
    {
      if (vertexTexCoord.x > iFillAmount * (iSpriteUV.z - iSpriteUV.x) + iSpriteUV.x)
        discard;
    }
    else if (iFillType == 2)
    {
      if (vertexTexCoord.x < (1 - iFillAmount) * (iSpriteUV.z - iSpriteUV.x) + iSpriteUV.x)
        discard;
    }
    else if (iFillType == 3)
    {
      if (vertexTexCoord.y > iFillAmount * (iSpriteUV.w - iSpriteUV.y) + iSpriteUV.y)
        discard;
    }
    else if (iFillType == 4)
    {
      if (vertexTexCoord.y < (1 - iFillAmount) * (iSpriteUV.w - iSpriteUV.y) + iSpriteUV.y)
        discard;
    }
    else if (iFillType == 5)
    {
      float rotAngle = clamp(0, 360, 360 * (1 - clamp(0, 1, iFillAmount)));
      float startAngle = iRRot - iREnd;
        float endAngle = iRRot + iREnd + rotAngle;
    
        // check offsets
        float offset0 = clamp(0, 360, startAngle + 360);
        float offset360 = clamp(0, 360, endAngle - 360);
    
        // convert uv to atan coordinates
        vec2 atan2Coord = vec2(
        mix(-1, 1, (vertexTexCoord.x - iSpriteUV.x) / (iSpriteUV.z - iSpriteUV.x)),
        mix(-1, 1, (vertexTexCoord.y - iSpriteUV.y) / (iSpriteUV.w - iSpriteUV.y)));
        float atanAngle = atan(atan2Coord.y, atan2Coord.x) * 57.3; // angle in degrees
    
        // convert angle to 360 system
        if(atanAngle < 0) atanAngle = 360 + atanAngle;
    
        if(atanAngle >= startAngle && atanAngle <= endAngle) discard;
        if(atanAngle <= offset360) discard;
        if(atanAngle >= offset0) discard;
    }
    else if (iFillType == 6)
    {
      float rotAngle = clamp(0, 360, 360 * (1 - clamp(0, 1, iFillAmount)));
      float startAngle = iRRot - iRStart - rotAngle;
      float endAngle = iRRot + iREnd;
      
      // check offsets
      float offset0 = clamp(0, 360, startAngle + 360);
      float offset360 = clamp(0, 360, endAngle - 360);
      
      // convert uv to atan coordinates
      vec2 atan2Coord = vec2(
      mix(-1, 1, (vertexTexCoord.x - iSpriteUV.x) / (iSpriteUV.z - iSpriteUV.x)),
      mix(-1, 1, (vertexTexCoord.y - iSpriteUV.y) / (iSpriteUV.w - iSpriteUV.y)));
      float atanAngle = atan(atan2Coord.y, atan2Coord.x) * 57.3; // angle in degrees
      
      // convert angle to 360 system
      if(atanAngle < 0) atanAngle = 360 + atanAngle;
      
      if(atanAngle >= startAngle && atanAngle <= endAngle) discard;
      if(atanAngle <= offset360) discard;
      if(atanAngle >= offset0) discard;
    }
  }
  vec2 offset = vec2(iSpriteUV.x, iSpriteUV.y);
  vec2 tiling = vec2(iSpriteUV.z - offset.x, iSpriteUV.w - offset.y);
  vec2 uv = vec2(vertexTexCoord.x * tiling.x + offset.x, vertexTexCoord.y * tiling.y + offset.y);
  color = texture(_MainTex, uv) * vertexColor * _Color;
}
