#include "Physics/Collider.h"

namespace DeltaEngine
{
    //BoxColliders
    BoxCollider::BoxCollider() :
        _aabb{ {1,1},{1,1} }
    {}
    BoxCollider::BoxCollider(Vector2 center, Vector2 size) :
        _aabb{center,size}
    {}
	bool BoxCollider::Intersection(const BoxCollider* aabb)
	{
        update_collider();
        return CollisionIntersection_RectRect_Static(_aabb, aabb->_aabb);
	}
    bool BoxCollider::Intersection(const CircleCollider* circle)
    {
        update_collider();
        return CollisionIntersection_RectCircle_Static(_aabb, circle->circle);
    }
    bool BoxCollider::Intersection(const RayCollider* ray)
    {
        update_collider();
        return CollisionIntersection_RectRay_Static(_aabb, ray->ray);
    }
    bool BoxCollider::Intersection(const LineCollider* linecollider)
    {
        update_collider();
        return CollisionIntersection_RectLine_Static(_aabb, linecollider->line);
    }
    ColliderType BoxCollider::GetType() const
    {
        return ColliderType::BOX;
    }
    void BoxCollider::OnDrawGizmos()
    {
        Gizmos::Draw2DWireBox(center);
    }
    void BoxCollider::update_collider()
    {
        _aabb.min = { center.x - size.x / 2, center.y - size.y / 2 };
        _aabb.max = { center.x + size.x / 2, center.y + size.y / 2 };
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
    bool CircleCollider::Intersection(const BoxCollider* aabb)
    {
        update_collider();
        return CollisionIntersection_RectCircle_Static(aabb->_aabb, circle);
    }
    bool CircleCollider::Intersection(const CircleCollider* circle1)
    {
        update_collider();
        return CollisionIntersecction_CircleCircle_Static(circle, circle1->circle);
    }
    bool CircleCollider::Intersection(const RayCollider* ray)
    {
        update_collider();
        float temp;
        return CollisionIntersection_RayCircle(ray->ray, circle, temp);
    }
    bool CircleCollider::Intersection(const LineCollider* line)
    {
        update_collider();
        return CollisionIntersection_CircleLineSegment_Static(circle, line->line);
    }
    ColliderType CircleCollider::GetType() const
    {
        return ColliderType::CIRCLE;
    }
    void CircleCollider::OnDrawGizmos()
    {
        Gizmos::Draw2DCircle(center);
    }
    void CircleCollider::update_collider()
    {
        circle.m_center = center;
        circle.m_radius = size.x;
    }

    //Line Colliders
    LineCollider::LineCollider() :
        line()
    {}
    LineCollider::LineCollider(Vector2 pt0, Vector2 pt1) :
        line{pt0,pt1}
    {}
    bool LineCollider::Intersection(const BoxCollider* aabb)
    {
        update_collider();
        return CollisionIntersection_RectLine_Static(aabb->_aabb, line);
    }
    bool LineCollider::Intersection(const CircleCollider* circle)
    {
        update_collider();
        return CollisionIntersection_CircleLineSegment_Static(circle->circle, line);
    }
    bool LineCollider::Intersection(const RayCollider* ray)
    {
        update_collider();
        return CollisionIntersection_RayLine_Static(ray->ray, line);
    }
    bool LineCollider::Intersection(const LineCollider* line2)
    {
        update_collider();
        return CollisionIntersection_LineLine_Static(line, line2->line);
    }
    ColliderType LineCollider::GetType() const
    {
        return ColliderType::LINE;
    }
    void LineCollider::OnDrawGizmos()
    {

    }
    void LineCollider::update_collider()
    {

       //need to do logic
    }

    //RayColliders
    RayCollider::RayCollider() :
        ray{}
    {}
    RayCollider::RayCollider(Vector2 pt, Vector2 dir) :
        ray{ pt,dir }
    {}
    bool RayCollider::Intersection(const BoxCollider* aabb)
    {
        update_collider();
        return CollisionIntersection_RectRay_Static(aabb->_aabb, ray);
    }
    bool RayCollider::Intersection(const CircleCollider* circle)
    {
        update_collider();
        float temp;
        return CollisionIntersection_RayCircle(ray, circle->circle, temp);
    }
    bool RayCollider::Intersection(const RayCollider* ray)
    {
        update_collider();
        return true; //not implemented yet, Ray to ray
    }
    bool RayCollider::Intersection(const LineCollider* line)
    {
        update_collider();
        return CollisionIntersection_RayLine_Static(ray, line->line);
    }

    ColliderType RayCollider::GetType() const
    {
        return ColliderType::LINE;
    }
    void RayCollider::OnDrawGizmos()
    {

    }
    void RayCollider::update_collider()
    {
        ray.m_pt0 = center;
    }


}