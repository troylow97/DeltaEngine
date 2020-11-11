#include "Transform.h"

namespace DeltaEngine
{
  Transform::Transform() :
    old_position{0.0f, 0.0f},
    position{0.0f, 0.0f},
    scale{1.0f, 1.0f, 1.0f},
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
