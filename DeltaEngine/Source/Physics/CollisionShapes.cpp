#include "Physics/CollisionShapes.h"

namespace DeltaEngine
{
	AABB::AABB() :
		min{ 0,0 },
		max{ 1,1 }
	{}

	AABB::AABB(Vector2 center, Vector2 size) :
		min{ center.x - size.x / 2, center.y - size.y / 2 },
		max{ center.x + size.x / 2, center.y + size.y / 2 }
	{}

	void AABB::calculate_position(Vector2 center, Vector2 size)
	{
		min = { center.x - size.x / 2, center.y - size.y / 2 };
		max = { center.x + size.x / 2, center.y + size.y / 2 };
	}

	void AABB::combine(const AABB& aabb)
	{
		if (min.x > aabb.min.x) { min.x = aabb.min.x; }
		if (min.y > aabb.min.y) { min.y = aabb.min.y; }
		if (max.x < aabb.max.x) { max.x = aabb.max.x; }
		if (max.y < aabb.max.y) { max.y = aabb.max.y; }
	}

	void AABB::combine(const AABB& aabb1, const AABB& aabb2)
	{
		if (aabb1.min.x > aabb2.min.x) { min.x = aabb2.min.x; }
		else { min.x = aabb1.min.x; }
		if (aabb1.min.y > aabb2.min.y) { min.y = aabb2.min.y; }
		else { min.y = aabb1.min.y; }
		if (aabb1.max.x < aabb2.max.x) { max.x = aabb2.max.x; }
		else { max.x = aabb1.max.x; }
		if (aabb1.max.y < aabb2.max.y) { max.y = aabb2.max.y; }
		else { max.y = aabb1.max.y; }
	}
	bool AABB::contains(const AABB& aabb) const
	{
		bool result = true;
		result = result && min.x <= aabb.min.x;
		result = result && min.y <= aabb.min.y;
		result = result && aabb.max.x <= max.x;
		result = result && aabb.max.y <= max.y;
		return result;
	}

	float AABB::GetPerimeter() const
	{
		float wx = max.x - min.x;
		float wy = max.y - min.y;
		return 2.0f * (wx + wy);
	}

	float AABB::getWidth() const
	{
		return max.x - min.x;
	}

	float AABB::getHeight() const
	{
		return max.y - min.y;
	}

	LineSegment::LineSegment() :
		m_pt0{ 0,0 },
		m_pt1{ 1,1 },
		m_normal{ 1,0 }
	{}

	LineSegment::LineSegment(const Vector2 pt0,const Vector2 pt1) :
		m_pt0{ pt0 },
		m_pt1{ pt1 },
		m_normal{ {pt1.y - pt0.y},{-(pt1.x - pt0.x)} }
	{}
	LineSegment::LineSegment(const Vector2& pos, const Vector2& scale, const Vector2& dir) 
	{
		m_pt0 = pos;
		Vector2 normalised_dir = dir;
		normalised_dir = Normalise(normalised_dir);
		normalised_dir *= scale;
		m_pt1 = m_pt0 + normalised_dir;
		m_normal = { m_pt1.y - m_pt0.y, -(m_pt1.x - m_pt0.x) };
	}

	Circle::Circle() :
		m_center{ 0,0 },
		m_radius{ 1 },
		m_mass{ 1.0f }
	{}

	Circle::Circle(const Vector2 center,const float radius,const float mass) :
		m_center{ center },
		m_radius{ radius },
		m_mass{ mass }
	{}

	Circle::Circle(const Vector2 center,const float radius) :
		m_center{ center },
		m_radius{ radius },
		m_mass{ 1.0f }
	{}

	Circle::Circle(const Vector2 center, const Vector2 radius) :
		m_center{ center },
		m_radius{ radius.x },
		m_mass{ 1.0f }
	{}

	Ray::Ray() :
		m_pt0{ 0,0 },
		m_dir{ 0,0 }
	{}

	Ray::Ray(const Vector2 pt,const Vector2 dir) :
		m_pt0{ pt },
		m_dir{ dir }
	{}

}


