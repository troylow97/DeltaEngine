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
    anchors[0] = Vector3(0, 0, 1);
    controlsLeft[0] = Vector3(-0.1f, 0, 0);
    controlsRight[0] = Vector3(0.1f, 0, 1);

    anchors[1] = Vector3(0.5f, 0.5f, 1);
    controlsLeft[1] = Vector3(-0.1f, 0, 1);
    controlsRight[1] = Vector3(0.1f, 0, 1);

    anchors[2] = Vector3(1, 1, 0);
    controlsLeft[2] = Vector3(-0.1f, 0, 1);
    controlsRight[2] = Vector3(0.1f, 0, 1);

    anchors[3] = Vector3(1, 1, 1);
    controlsLeft[3] = Vector3(-0.1f, 0, 1);
    controlsRight[3] = Vector3(0.1f, 0, 0);
  }
  BezierCurve::BezierCurve(int constant)
  {
    min = constant - 1.f;
    max = constant + 1.f;

    anchors[0] = Vector3(0, 0.5f, 1);
    controlsLeft[0] = Vector3(-0.1f, 0, 0);
    controlsRight[0] = Vector3(0.1f, 0, 0);

    anchors[1] = Vector3(0.5f, 0.5f, 1);
    controlsLeft[1] = Vector3(-0.1f, 0, 1);
    controlsRight[1] = Vector3(0.1f, 0, 1);

    anchors[2] = Vector3(1, 1, 0);
    controlsLeft[2] = Vector3(-0.1f, 0, 1);
    controlsRight[2] = Vector3(0.1f, 0, 1);

    anchors[3] = Vector3(1, 0.5f, 1);
    controlsLeft[3] = Vector3(-0.1f, 0, 0);
    controlsRight[3] = Vector3(0.1f, 0, 0);
  }

  float BezierCurve::Evaluate(float time)
  {
    int i = 0;
    float firstCheck = 0, lastCheck = 1;
    int startInd = 0, endInd = -1;
    for (auto position : anchors)
    {
      position.z = 1;
      if (!position.z)
        continue;
      if (position.x < time)
      {
        if (position.x > firstCheck)
        {
          firstCheck = position.x;
          startInd = i;
        }
      }
      else
      {
        if (position.x <= lastCheck)
        {
          lastCheck = position.x;
          endInd = i;
        }
      }
      ++i;
    }

    if (endInd == 0)
      return anchors[0].y;
    else if (endInd == -1)
      return anchors.back().y;

    float t0 = anchors[startInd].x;
    float t1 = anchors[endInd].x;
    time = (time - t0) / (t1 - t0);
    bool right = controlsRight[startInd].z;
    bool left = controlsLeft[endInd].z;

    if (left && right)
    {
      Vector2 p0 = anchors[startInd];
      Vector2 p1 = anchors[startInd] + controlsRight[startInd];
      Vector2 p2 = anchors[endInd] + controlsLeft[endInd];
      Vector2 p3 = anchors[endInd];

      return min + (max - min) * EvaluateCubic(p0, p1, p2, p3, time).y;
    }
    else if (left || right)
    {
      Vector2 p0 = anchors[startInd];
      Vector2 p1 = left ?
        anchors[endInd] + controlsLeft[endInd] :
        anchors[startInd] + controlsRight[startInd];
      Vector2 p2 = anchors[endInd];

      return min + (max - min) * EvaluateQuadratic(p0, p1, p2, time).y;
    }
    else
    {
      Vector2 p0 = anchors[startInd];
      Vector2 p1 = anchors[endInd];
      return min + (max - min) * EvaluateLinear(p0, p1, time).y;
    }
  }
  void BezierCurve::Sort()
  {
    for (size_t i = 0; i < 4; ++i)
      for (size_t j = 0; j < 3 - i; ++j)
        if (anchors[j].z < 0 || anchors[j].z > anchors[j + 1].z)
        {
          auto temp1 = anchors[j];
          anchors[j] = anchors[j + 1];
          anchors[j + 1] = temp1;

          auto temp2 = controlsLeft[j];
          controlsLeft[j] = controlsLeft[j + 1];
          controlsLeft[j + 1] = temp2;

          auto temp3 = controlsRight[j];
          controlsRight[j] = controlsRight[j + 1];
          controlsRight[j + 1] = temp3;
        }
  }

  BezierRange::BezierRange(int constant)
  {
    min = constant;
    max = constant;
  }

  BezierRange3::BezierRange3(int constant)
  {
    minX = constant;
    maxX = constant;
    minY = constant;
    maxY = constant;
    minZ = constant;
    maxZ = constant;
  }
}
