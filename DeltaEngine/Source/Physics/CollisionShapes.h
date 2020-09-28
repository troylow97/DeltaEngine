#pragma once
#include "DEpch.h"
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
	// Ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
	//struct DE_API RayCastInput
	//{
	//	Vector2 p1, p2;
	//	float maxFraction;
	//};
	//
	///// Ray-cast output data. The ray hits at p1 + fraction * (p2 - p1), where p1 and p2 come from RayCastInput.
	//struct DE_API RayCastOutput
	//{
	//	Vector2 normal;
	//	float fraction;
	//};

	struct DE_API AABB
	{
	public:
		Vector2 min;
		Vector2 max;
		AABB();
		AABB(Vector2 center, Vector2 size);
		void combine(const AABB& aabb);
		void combine(const AABB& aabb1, const AABB& aabb2);
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
	
		bool contains(const AABB& aabb) const;

		/// Get the perimeter length
		float GetPerimeter() const;
	};

	struct DE_API LineSegment
	{
		LineSegment();
		LineSegment(Vector2 pt0, Vector2 pt1);
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
		Circle(Vector2 center, float radius, float mass);
		Circle(Vector2 center, float radius);
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