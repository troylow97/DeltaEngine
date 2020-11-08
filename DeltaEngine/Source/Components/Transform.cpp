#include "Transform.h"

namespace DeltaEngine
{
  Transform::Transform() :
    old_position{0.0, 0.0},
    position{0.0, 0.0},
    scale{1.0, 1.0},
    rotation{0, 0, 0, 1}
  {
  };

  Transform::Transform(Vector3 pos, Quaternion rot, Vector3 sca)
    : position{pos}, scale{sca}, rotation{rot}
  {
  }

  Matrix4x4 Transform::LocalToWorldMatrix()
  {
    Vector3 pos = position;
    Quaternion rot = rotation;
    Vector3 sca = scale;

    return Matrix4x4::Scale(sca) * Matrix4x4::Rotate(rot) * Matrix4x4::Transpose(Matrix4x4::Translate(pos));
  }
}
