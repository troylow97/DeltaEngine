#include "Gradient.h"
#include "Math.h"

namespace DeltaEngine
{
  Gradient::Gradient()
  {
    colorKeys[0] = Color(1,1,1,0);
    colorKeys[1] = Color(1,1,1,1);
    colorKeys[2] = Color(1,1,1,-1);
    colorKeys[3] = Color(1,1,1,-1);

    alphaKeys[0] = Vector2(1, 0 );
    alphaKeys[1] = Vector2(1, 1 );
    alphaKeys[2] = Vector2(1, -1);
    alphaKeys[3] = Vector2(1, -1);
  }

  Color Gradient::Evaluate(float time)
  {
    Color ret = Color();

    int i = 0;
    float firstCheck = 0, lastCheck = 1;
    int startInd = 0, endInd = -1;
    for (auto color : colorKeys)
    {
      if (color.a < 0)
        continue;
      if (color.a < time)
      {
        if (color.a > firstCheck)
        {
          firstCheck = color.a;
          startInd = i;
        }
      }
      else
      {
        if (color.a <= lastCheck)
        {
          lastCheck = color.a;
          endInd = i;
        }
      }
      ++i;
    }

    if (endInd == 0)
      ret = colorKeys[0];
    else if (endInd == -1)
      ret = colorKeys.back();
    else
    {
      float t0 = colorKeys[startInd].a;
      float t1 = colorKeys[endInd].a;
      float t = (time - t0) / (t1 - t0);
      ret = Color::Lerp(colorKeys[startInd], colorKeys[endInd], t);
    }

    i = 0;
    firstCheck = 0, lastCheck = 1;
    startInd = 0, endInd = -1;
    for (auto value : alphaKeys)
    {
      if (value.y < 0)
        continue;
      if (value.y < time)
      {
        if (value.y > firstCheck)
        {
          firstCheck = value.y;
          startInd = i;
        }
      }
      else
      {
        if (value.y <= lastCheck)
        {
          lastCheck = value.y;
          endInd = i;
        }
      }
      ++i;
    }

    if (endInd == 0)
      ret.a = alphaKeys[0].x;
    else if (endInd == -1)
      ret.a = alphaKeys.back().x;
    else
    {
      float t0 = alphaKeys[startInd].y;
      float t1 = alphaKeys[endInd].y;
      float t = (time - t0) / (t1 - t0);
      ret.a = Math::Lerp(alphaKeys[startInd].x, alphaKeys[endInd].x, t);
    }

    return ret;
  }
}
