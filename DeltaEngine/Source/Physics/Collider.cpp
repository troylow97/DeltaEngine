#include "Collider.h"

namespace DeltaEngine
{
    //BoxColliders
    BoxCollider::BoxCollider() :
        _aabb{ {1,1},{1,1} }
    {}
    BoxCollider::BoxCollider(float x, float y, Vector2 size) :
        _aabb{ {x - size.x / 2, y - size.y / 2},{x + size.x / 2, y + size.y / 2} }
    {}
	bool BoxCollider::Intersection(const BoxCollider& aabb)
	{
		return CollisionIntersection_RectRect_Static(_aabb, aabb._aabb);
	}
    bool BoxCollider::Intersection(const CircleCollider& circle)
    {
        return CollisionIntersection_RectCircle_Static(_aabb, circle.circle);
    }
    bool BoxCollider::Intersection(const RayCollider& ray)
    {
        return CollisionIntersection_RectRay(_aabb, ray.ray);
    }
    bool BoxCollider::Intersection(const LineCollider& linecollider)
    {
        return CollisionIntersection_RectLine_Static(_aabb, linecollider.line);
    }
    ColliderType BoxCollider::GetType() const
    {
        return ColliderType::BOX;
    }

    //CircleColliders
    bool CircleCollider::Intersection(const BoxCollider& aabb)
    {
        return CollisionIntersection_RectCircle_Static(aabb._aabb, circle);
    }
    bool CircleCollider::Intersection(const CircleCollider& circle1)
    {
        return CollisionIntersecction_CircleCircle_Static(circle, circle1.circle);
    }
    bool CircleCollider::Intersection(const RayCollider& ray)
    {
        float temp;
        return CollisionIntersection_RayCircle(ray.ray, circle, temp);
    }
    bool CircleCollider::Intersection(const LineCollider& line)
    {
        float temp;
        return CollisionIntersection_CircleLineSegment_Static(circle, line.line);
    }
    ColliderType CircleCollider::GetType() const
    {
        return ColliderType::CIRCLE;
    }

    //Line Colliders
    bool LineCollider::Intersection(const BoxCollider& aabb)
    {
        return CollisionIntersection_RectLine_Static(aabb._aabb, line);
    }
    bool LineCollider::Intersection(const CircleCollider& circle)
    {
        return CollisionIntersection_CircleLineSegment_Static(circle.circle, line);
    }
    bool LineCollider::Intersection(const RayCollider& ray)
    {
        return CollisionIntersection_RayLine(ray.ray, line);
    }
    bool LineCollider::Intersection(const LineCollider& line2)
    {
        return CollisionIntersection_LineLine(line, line2.line);
    }
    ColliderType LineCollider::GetType() const
    {
        return ColliderType::LINE;
    }

    //RayColliders
    bool RayCollider::Intersection(const BoxCollider& aabb)
    {
        return CollisionIntersection_RectRay(aabb._aabb, ray);
    }
    bool RayCollider::Intersection(const CircleCollider& circle)
    {
        float temp;
        return CollisionIntersection_RayCircle(ray, circle.circle, temp);
    }
    bool RayCollider::Intersection(const RayCollider& ray)
    {
        return true; //not implemented yet, Ray to ray
    }
    bool RayCollider::Intersection(const LineCollider& line)
    {
        return CollisionIntersection_RayLine(ray, line.line);
    }

    ColliderType RayCollider::GetType() const
    {
        return ColliderType::LINE;
    }

}