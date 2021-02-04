#include "BezierCurve.h"
#include "Math.h"

namespace DeltaEngine
{
  Vector2 EvaluateLinear(Vector2 a, Vector2 b, float t)
  {
    return Vector2::Lerp(a, b, t);
  }
  Vector2 EvaluateQuadratic(Vector2 a, Vector2 b, Vector2 c, float t)
  {
    Vector2 p0 = Vector2::Lerp(a, b, t);
    Vector2 p1 = Vector2::Lerp(b, c, t);
    return Vector2::Lerp(p0, p1, t);
  }
  Vector2 EvaluateCubic(Vector2 a, Vector2 b, Vector2 c, Vector2 d, float t)
  {
    Vector2 p0 = EvaluateQuadratic(a, b, c, t);
    Vector2 p1 = EvaluateQuadratic(b, c, d, t);
    return Vector2::Lerp(p0, p1, t);
  }

  BezierCurve::BezierCurve()
  {
    anchors[0] = { Vector2(0, 0), true };
    controlsLeft[0] = { Vector2(-0.1f, 0), false };
    controlsRight[0] = { Vector2(0.1f, 0), true };

    anchors[1] = { Vector2(0.5f, 0.5f), true };
    controlsLeft[1] = { Vector2(-0.1f, 0), true };
    controlsRight[1] = { Vector2(0.1f, 0), true };

    anchors[2] = { Vector2(1, 1), true };
    controlsLeft[2] = { Vector2(-0.1f, 0), true };
    controlsRight[2] = { Vector2(0.1f, 0), false };
  }
  BezierCurve::BezierCurve(int constant)
  {
    min = constant - 1.f;
    max = constant + 1.f;

    anchors[0] = { Vector2(0, 0.5f), true };
    controlsLeft[0] = { Vector2(-0.1f, 0), false };
    controlsRight[0] = { Vector2(0.1f, 0), false };

    anchors[1] = { Vector2(1, 0.5f), true };
    controlsLeft[1] = { Vector2(-0.1f, 0), false };
    controlsRight[1] = { Vector2(0.1f, 0), false };
  }

  float BezierCurve::Evaluate(float time)
  {
    int i = 0;
    float lastCheck = 0;
    int startInd = -1, endInd = -1;
    for (auto [position, active] : anchors)
    {
      if (!active)
        continue;
      if (position.x < time)
      {
        if (position.x > lastCheck)
        {
          lastCheck = position.x;
          startInd = i;
        }
      }
      else
      {
        endInd = i;
        break;
      }
      ++i;
    }
    
    if (startInd == -1)
      return anchors[0].first.y;
    if (endInd == -1)
      return anchors.back().first.y;

    float t0 = anchors[startInd].first.x;
    float t1 = anchors[endInd].first.x;
    time = (time - t0) / (t1 - t0);
    bool right = controlsRight[startInd].second;
    bool left = controlsLeft[endInd].second;

    if (left && right)
    {
      Vector2 p0 = anchors[startInd].first;
      Vector2 p1 = anchors[startInd].first + controlsRight[startInd].first;
      Vector2 p2 = anchors[endInd].first + controlsLeft[endInd].first;
      Vector2 p3 = anchors[endInd].first;

      return min + (max - min) * EvaluateCubic(p0, p1, p2, p3, time).y;
    }
    else if (left || right)
    {
      Vector2 p0 = anchors[startInd].first;
      Vector2 p1 = left ?
        anchors[endInd].first + controlsLeft[endInd].first :
        anchors[startInd].first + controlsRight[startInd].first;
      Vector2 p2 = anchors[endInd].first;

      return min + (max - min) * EvaluateQuadratic(p0, p1, p2, time).y;
    }
    else
    {
      Vector2 p0 = anchors[startInd].first;
      Vector2 p1 = anchors[endInd].first;
      return min + (max - min) * EvaluateLinear(p0, p1, time).y;
    }
  }
}
