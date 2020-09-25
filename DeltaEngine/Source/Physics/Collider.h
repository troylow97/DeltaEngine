#pragma once
#include "Collision.h"

namespace DeltaEngine
{

  enum class DE_API ColliderType
  {
    NONE,
    BOX,
    CIRCLE,
    LINE,
    RAY
  };

  class DE_API BoxCollider;
  class DE_API CircleCollider;
  class DE_API RayCollider;
  class DE_API LineCollider;

  class DE_API Collider
  {
  public:
    // if doing trigger
    // bool isTrigger{false};
    Vector2 center;
    ColliderType shape;

    // If doing AABB Tree]
    // AABB FatAABB() = 0;
    // AABB AABB() = 0;

  public:
    Collider() : center{ 0,0 }, shape{ ColliderType::NONE } {}
    virtual ~Collider() = default;
    virtual ColliderType GetType() const = 0;
    virtual bool Intersection(const BoxCollider& aabb) = 0;
    virtual bool Intersection(const CircleCollider& circle) = 0;
    virtual bool Intersection(const RayCollider& ray) = 0;
    virtual bool Intersection(const LineCollider& line) = 0;
  };

  class DE_API BoxCollider : public Collider
  {
  public:
      AABB _aabb;
      Vector2 size; // Collider size
      BoxCollider();
      BoxCollider(Vector2, Vector2);
      virtual bool Intersection(const BoxCollider& aabb);
      virtual bool Intersection(const CircleCollider& circle);
      virtual bool Intersection(const RayCollider& ray);
      virtual bool Intersection(const LineCollider& linecollider);
      virtual ColliderType GetType() const;
  };

  class DE_API CircleCollider : public Collider
  {
  public:
      Circle circle;
      CircleCollider();
      CircleCollider(Vector2,float);
      CircleCollider(Vector2, float,float);
      virtual bool Intersection(const BoxCollider& aabb);
      virtual bool Intersection(const CircleCollider& circle1);
      virtual bool Intersection(const RayCollider& ray);
      virtual bool Intersection(const LineCollider& line);
      virtual ColliderType GetType() const;
  };
  
  class DE_API LineCollider : public Collider
  {
  public:
      LineSegment line;
      LineCollider();
      LineCollider(Vector2, Vector2);
      virtual bool Intersection(const BoxCollider& aabb);
      virtual bool Intersection(const CircleCollider& circle);
      virtual bool Intersection(const RayCollider& ray);
      virtual bool Intersection(const LineCollider& line2);
      virtual ColliderType GetType() const;
  };
  
  class DE_API RayCollider : public Collider
  {
  public:
      Ray ray;
      RayCollider();
      RayCollider(Vector2, Vector2);
      virtual bool Intersection(const BoxCollider& aabb);
      virtual bool Intersection(const CircleCollider& circle);
      virtual bool Intersection(const RayCollider& ray);
      virtual bool Intersection(const LineCollider& line);
      virtual ColliderType GetType() const;
  };

} // namespace DeltaEngine
