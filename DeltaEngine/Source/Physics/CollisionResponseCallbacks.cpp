#include "CollisionResponseCallbacks.h"
#include "Core/Math/Math.h"
#include <algorithm>

namespace DeltaEngine
{
	void AABB_CollisionResponse(Collider& obj1,Collider& obj2,Vector2& obj1_vel,Vector2& obj2_vel)
	{
        //Vector2 distance = CalculateAabbDistanceTo(obj1, obj2);
        //float shortestTime = 0;
        //float xAxisTimeToCollide = obj2_vel.x != 0 ? std::abs(distance.x/obj2_vel.x) : 0;
        //float yAxisTimeToCollide = obj2_vel.x != 0 ? std::abs(distance.y / obj2_vel.y) : 0;
        //
        //if (obj2_vel.x != 0 && obj2_vel.y == 0)
        //{
        //    // Colliison on X-axis only
        //    shortestTime = xAxisTimeToCollide;
        //    obj1_vel.x = obj2_vel.x * shortestTime;
        //}
        //else if (obj2_vel.x == 0 && obj2_vel.y != 0)
        //{
        //    // Collision on Y-axis only
        //    shortestTime = yAxisTimeToCollide;
        //    obj1_vel.y = obj2_vel.y * shortestTime;
        //}
        //else
        //{
        //    // Collision on X and Y axis (eg. slide up against a wall)
        //    shortestTime = Math::math_min(std::abs(xAxisTimeToCollide), std::abs(yAxisTimeToCollide));
        //
        //    obj1_vel.x = shortestTime * obj2_vel.x;
        //    obj1_vel.y = shortestTime * obj2_vel.y;
        //}

        obj1_vel.x = 0;
        obj1_vel.y = 0;
        obj2_vel.x = 0;
        obj2_vel.y = 0;

	}

   Vector2 CalculateAabbDistanceTo(Collider& e1,Collider& e2)
    {
        float dx = 0;
        float dy = 0;

        if (e1.center.x < e2.center.x)
        {
            dx = e2.center.x - (e1.center.x + e1.size.x);
        }
        else if (e1.center.x > e2.center.x)
        {
            dx = e1.center.x - (e2.center.x + e2.size.x);
        }

        if (e1.center.y < e2.center.y)
        {
            dy = e2.center.y - (e1.center.y + e1.size.y);
        }
        else if (e1.center.y > e2.center.y)
        {
            dy = e1.center.y - (e2.center.y + e2.size.y);
        }

        return { dx,dy };
    }
}