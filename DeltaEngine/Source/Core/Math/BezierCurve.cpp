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
    anchors.push_back(Vector2(0, 0));
    controlsLeft.push_back({ Vector2(-0.1f, 0), false });
    controlsRight.push_back({ Vector2(0.1f, 0), true });

    anchors.push_back(Vector2(0.5f, 0.5f));
    controlsLeft.push_back({ Vector2(-0.1f, 0), true });
    controlsRight.push_back({ Vector2(0.1f, 0), true });

    anchors.push_back(Vector2(1, 1));
    controlsLeft.push_back({ Vector2(-0.1f, 0), true });
    controlsRight.push_back({ Vector2(0.1f, 0), false });
  }

  float BezierCurve::Evaluate(float time)
  {
    int i = 0;
    int startInd = -1, endInd = -1;
    for (auto position : anchors)
    {
      if (position.x < time)
        startInd = i;
      else
      {
        endInd = i;
        break;
      }
      ++i;
    }
    
    if (startInd == -1)
      return anchors[0].y;
    if (endInd == -1)
      return anchors.back().y;

    float t0 = anchors[startInd].x;
    float t1 = anchors[endInd].x;
    time = (time - t0) / (t1 - t0);
    bool right = controlsRight[startInd].second;
    bool left = controlsLeft[endInd].second;

    if (left && right)
    {
      Vector2 p0 = anchors[startInd];
      Vector2 p1 = anchors[startInd] + controlsRight[startInd].first;
      Vector2 p2 = anchors[endInd] + controlsLeft[endInd].first;
      Vector2 p3 = anchors[endInd];

      return EvaluateCubic(p0, p1, p2, p3, time).y;
    }
    else if (left || right)
    {
      Vector2 p0 = anchors[startInd];
      Vector2 p1 = left ?
        anchors[endInd] + controlsLeft[endInd].first :
        anchors[startInd] + controlsRight[startInd].first;
      Vector2 p2 = anchors[endInd];

      return EvaluateQuadratic(p0, p1, p2, time).y;
    }
    else
    {
      Vector2 p0 = anchors[startInd];
      Vector2 p1 = anchors[endInd];
      return EvaluateLinear(p0, p1, time).y;
    }
  }
}
