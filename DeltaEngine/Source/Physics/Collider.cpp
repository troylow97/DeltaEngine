#include "Collider.h"

namespace DeltaEngine
{
    //BoxColliders
    BoxCollider::BoxCollider() :
        _aabb{ {1,1},{1,1} }
    {}
    BoxCollider::BoxCollider(Vector2 center, Vector2 size) :
        _aabb{center,size}
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
        return CollisionIntersection_RectRay_Static(_aabb, ray.ray);
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
    CircleCollider::CircleCollider() :
        circle{ {0,0},1.0f,1.0f }
    {}
    CircleCollider::CircleCollider(Vector2 center, float radius) :
        circle{ center,radius, 1.0f }
    {}
    CircleCollider::CircleCollider(Vector2 center, float radius,float mass) :
        circle{ center,radius, mass }
    {}
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
    LineCollider::LineCollider() :
        line()
    {}
    LineCollider::LineCollider(Vector2 pt0, Vector2 pt1) :
        line{pt0,pt1}
    {}
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
        return CollisionIntersection_RayLine_Static(ray.ray, line);
    }
    bool LineCollider::Intersection(const LineCollider& line2)
    {
        return CollisionIntersection_LineLine_Static(line, line2.line);
    }
    ColliderType LineCollider::GetType() const
    {
        return ColliderType::LINE;
    }

    //RayColliders
    RayCollider::RayCollider() :
        ray{}
    {}
    RayCollider::RayCollider(Vector2 pt, Vector2 dir) :
        ray{ pt,dir }
    {}
    bool RayCollider::Intersection(const BoxCollider& aabb)
    {
        return CollisionIntersection_RectRay_Static(aabb._aabb, ray);
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
        return CollisionIntersection_RayLine_Static(ray, line.line);
    }

    ColliderType RayCollider::GetType() const
    {
        return ColliderType::LINE;
    }

}