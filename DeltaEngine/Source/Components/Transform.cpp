#include "Transform.h"

namespace DeltaEngine
{
Transform::Transform() : position {}, rotation {}, scale { 1, 1, 1 } {}
Transform::Transform( Vector3 pos, Quaternion rot, Vector3 sca )
  : position { pos }, rotation { rot }, scale { sca } {}

Matrix4x4 Transform::LocalToWorldMatrix()
{
  Vector3 pos = position;
  Quaternion rot = rotation;
  Vector3 sca = scale;

  return Matrix4x4::Scale( sca ) * Matrix4x4::Rotate( rot ) * Matrix4x4::Transpose( Matrix4x4::Translate( pos ) );
}
}
