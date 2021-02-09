#include "Gradient.h"
#include "Math.h"

namespace DeltaEngine
{
  Gradient::Gradient()
  {
    //colorKeys[0] = {Color(), 0} ;
    //colorKeys[1] = {Color(), 1} ;
    //
    //alphaKeys[0] = {1, 0} ;
    //alphaKeys[1] = {1, 1} ;
  }

  Color Gradient::Evaluate(float time)
  {
    Color ret = Color();

    int i = 0;
    int startInd = -1, endInd = -1;
    for (auto [color, location] : colorKeys)
    {
      if (location < time)
        startInd = i;
      else
      {
        endInd = i;
        break;
      }
      ++i;
    }

    if (startInd == -1)
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
    startInd = -1, endInd = -1;
    for (auto [value, location] : alphaKeys)
    {
      if (location < time)
        startInd = i;
      else
      {
        endInd = i;
        break;
      }
      ++i;
    }

    if (startInd == -1)
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
