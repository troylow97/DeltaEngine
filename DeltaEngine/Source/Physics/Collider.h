#pragma once

namespace DeltaEngine
{

  enum class ColliderType
  {
    BOX
  };

  class Collider
  {
  public:
    // if doing trigger
    // bool isTrigger{false};
    Vector3 center{};

    // If doing AABB Tree]
    // AABB FatAABB() = 0;
    // AABB AABB() = 0;

  protected:
    Collider() = default;
    Collider(const Vector3 center) : center{center} {}
    virtual ColliderType GetType() const = 0;
    virtual bool Intersection(Collider *other) = 0;
    // AABB _aabb;
    // AABB _fatAABB;
  };

} // namespace DeltaEngine