#include "Gradient.h"
#include "Math.h"

namespace DeltaEngine
{
  Gradient::Gradient()
  {
    colorKeys[0] = { Color(1,1,1,1), 0 };
    colorKeys[1] = { Color(1,1,1,1), 0.5f };
    colorKeys[2] = { Color(1,1,1,1), 1 };
    colorKeys[3] = { Color(1,1,1,1), -1 };
    colorKeys[4] = { Color(1,1,1,1), -1 };
    colorKeys[5] = { Color(1,1,1,1), -1 };
    colorKeys[6] = { Color(1,1,1,1), -1 };
    colorKeys[7] = { Color(1,1,1,1), -1 };

    alphaKeys[0] = { 1, 0 };
    alphaKeys[1] = { 1, 1 };
    alphaKeys[2] = { 1, -1 };
    alphaKeys[3] = { 1, -1 };
    alphaKeys[4] = { 1, -1 };
    alphaKeys[5] = { 1, -1 };
    alphaKeys[6] = { 1, -1 };
    alphaKeys[7] = { 1, -1 };
  }

  Color Gradient::Evaluate(float time)
  {
    Color ret = Color();

    int i = 0;
    float firstCheck = 0, lastCheck = 1;
    int startInd = 0, endInd = -1;
    for (auto [color, location] : colorKeys)
    {
      if (location < 0)
        continue;
      if (location < time)
      {
        if (location > firstCheck)
        {
          firstCheck = location;
          startInd = i;
        }
      }
      else
      {
        if (location <= lastCheck)
        {
          lastCheck = location;
          endInd = i;
        }
      }
      ++i;
    }

    if (endInd == 0)
      ret = colorKeys[0].first;
    else if (endInd == -1)
      ret = colorKeys.back().first;
    else
    {
      float t0 = colorKeys[startInd].second;
      float t1 = colorKeys[endInd].second;
      float t = (time - t0) / (t1 - t0);
      ret = Color::Lerp(colorKeys[startInd].first, colorKeys[endInd].first, t);
    }

    i = 0;
    firstCheck = 0, lastCheck = 1;
    startInd = 0, endInd = -1;
    for (auto [value, location] : alphaKeys)
    {
      if (location < 0)
        continue;
      if (location < time)
      {
        if (location > firstCheck)
        {
          firstCheck = location;
          startInd = i;
        }
      }
      else
      {
        if (location <= lastCheck)
        {
          lastCheck = location;
          endInd = i;
        }
      }
      ++i;
    }

    if (endInd == 0)
      ret.a = alphaKeys[0].first;
    else if (endInd == -1)
      ret.a = alphaKeys.back().first;
    else
    {
      float t0 = alphaKeys[startInd].second;
      float t1 = alphaKeys[endInd].second;
      float t = (time - t0) / (t1 - t0);
      ret.a = Math::Lerp(alphaKeys[startInd].first, alphaKeys[endInd].first, t);
    }

    return ret;
  }
}
