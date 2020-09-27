#pragma once
#include "DEpch.h"
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
	// Ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
	struct DE_API RayCastInput
	{
		Vector2 p1, p2;
		float maxFraction;
	};

	/// Ray-cast output data. The ray hits at p1 + fraction * (p2 - p1), where p1 and p2 come from RayCastInput.
	struct DE_API RayCastOutput
	{
		Vector2 normal;
		float fraction;
	};

	struct DE_API AABB
	{
	public:
		Vector2 min;
		Vector2 max;
		AABB() :
			min{ 0,0 },
			max{ 1,1 }
		{}
		AABB(Vector2 center, Vector2 size) :
			min{ center.x - size.x / 2, center.y - size.y / 2 },
			max{ center.x + size.x / 2, center.y + size.y / 2 }
		{}
		void combine(const AABB& aabb)
		{
			if (min.x > aabb.min.x) { min.x = aabb.min.x; }
			if (min.y > aabb.min.y) { min.y = aabb.min.y; }
			if (max.x < aabb.max.x) { max.x = aabb.max.x; }
			if (max.y < aabb.max.y) { max.y = aabb.max.y; }
		}
		void combine(const AABB& aabb1, const AABB& aabb2)
		{
			if (aabb1.min.x > aabb2.min.x) { min.x = aabb2.min.x; } else { min.x = aabb1.min.x; }
			if (aabb1.min.y > aabb2.min.y) { min.y = aabb2.min.y; } else { min.y = aabb1.min.y; }
			if (aabb1.max.x < aabb2.max.x) { max.x = aabb2.max.x; } else { max.x = aabb1.max.x; }
			if (aabb1.max.y < aabb2.max.y) { max.y = aabb2.max.y; } else { max.y = aabb1.max.y; }
		}
		//bool RayCast(RayCastOutput* output, const RayCastInput& input) const
		//{
		//	float tmin = -b2_maxFloat;
		//	float tmax = b2_maxFloat;
		//
		//	Vector2 p = input.p1;
		//	Vector2 d = input.p2 - input.p1;
		//	Vector2 absD = b2Abs(d);
		//
		//	Vector2 normal;
		//
		//	for (int i = 0; i < 2; ++i)
		//	{
		//		if (absD(i) < b2_epsilon)
		//		{
		//			// Parallel.
		//			if (p(i) < min(i) || max(i) < p(i))
		//			{
		//				return false;
		//			}
		//		}
		//		else
		//		{
		//			float inv_d = 1.0f / d(i);
		//			float t1 = (min(i) - p(i)) * inv_d;
		//			float t2 = (max(i) - p(i)) * inv_d;
		//
		//			// Sign of the normal vector.
		//			float s = -1.0f;
		//
		//			if (t1 > t2)
		//			{
		//				std::swap(t1, t2);
		//				s = 1.0f;
		//			}
		//
		//			// Push the min up
		//			if (t1 > tmin)
		//			{
		//				normal.SetZero();
		//				normal(i) = s;
		//				tmin = t1;
		//			}
		//
		//			// Pull the max down
		//			tmax = std::min(tmax, t2);
		//
		//			if (tmin > tmax)
		//			{
		//				return false;
		//			}
		//		}
		//}
	
		bool contains(const AABB& aabb) const
		{
			bool result = true;
			result = result && min.x <= aabb.min.x;
			result = result && min.y <= aabb.min.y;
			result = result && aabb.max.x <= max.x;
			result = result && aabb.max.y <= max.y;
			return result;
		}

		/// Get the perimeter length
		float GetPerimeter() const
		{
			float wx = max.x - min.x;
			float wy = max.y - min.y;
			return 2.0f * (wx + wy);
		}
	};

	struct DE_API LineSegment
	{
		LineSegment() :
			m_pt0{ 0,0 },
			m_pt1{ 1,1 },
			m_normal{ 1,0 }
		{}
		LineSegment(Vector2 pt0, Vector2 pt1) :
			m_pt0{ pt0 },
			m_pt1{ pt1 },
			m_normal{ {pt1.x - pt0.x},{pt1.y - pt0.y} }
		{}
		Vector2	m_pt0;
		Vector2	m_pt1;
		Vector2	m_normal; //outward
	};

	struct DE_API Circle
	{
		Circle() :
			m_center{ 0,0 },
			m_radius{ 1.0f },
			m_mass{ 1.0f }
		{}
		Circle(Vector2 center, float radius, float mass) :
			m_center{ center },
			m_radius{ radius },
			m_mass{ mass }
		{}
		Circle(Vector2 center, float radius) :
			m_center{ center },
			m_radius{ radius },
			m_mass{ 1.0f }
		{}
		Vector2  m_center;
		float	m_radius;
		float   m_mass;
	};

	struct DE_API Ray
	{
		Ray() :
			m_pt0{ 0,0 },
			m_dir{ 0,0 }
		{}
		Ray(Vector2 pt, Vector2 dir) :
			m_pt0{ pt },
			m_dir{ dir }
		{}
		Vector2	m_pt0;
		Vector2	m_dir;
	};
}