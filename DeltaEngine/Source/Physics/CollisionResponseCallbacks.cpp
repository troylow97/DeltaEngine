#include "CollisionResponseCallbacks.h"
#include "Core/Math/Math.h"
#include <algorithm>

namespace DeltaEngine
{
	void AABB_CollisionResponse(Collider& obj1,Collider& obj2,Vector2& obj1_vel,Vector2& obj2_vel)
	{
        Vector2 distance = CalculateAabbDistanceTo(obj1, obj2);
        float shortestTime = 0;
        float xAxisTimeToCollide = obj2_vel.x != 0 ? std::abs(distance.x/obj2_vel.x) : 0;
        float yAxisTimeToCollide = obj2_vel.x != 0 ? std::abs(distance.y / obj2_vel.y) : 0;
        
        if (obj2_vel.x != 0 && obj2_vel.y == 0)
        {
            // Colliison on X-axis only
            shortestTime = xAxisTimeToCollide;
            obj1_vel.x = obj2_vel.x * shortestTime;
        }
        else if (obj2_vel.x == 0 && obj2_vel.y != 0)
        {
            // Collision on Y-axis only
            shortestTime = yAxisTimeToCollide;
            obj1_vel.y = obj2_vel.y * shortestTime;
        }
        else
        {
            // Collision on X and Y axis (eg. slide up against a wall)
            shortestTime = Math::math_min(std::abs(xAxisTimeToCollide), std::abs(yAxisTimeToCollide));
        
            obj1_vel.x = shortestTime * obj2_vel.x;
            obj1_vel.y = shortestTime * obj2_vel.y;
        }


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

   void CollisionResponse_CircleCircle(Collider& col1,RigidBody& r1,Collider& col2, RigidBody& r2)
   {
       
       //get the direction of reflection using dot product
       float aA = Vector2DotProduct(r1.Velocity, col1.normal);
       float aB = Vector2DotProduct(r2.Velocity, col2.normal);

       //calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
       //col1.ReflectionVector = r1.Velocity - (2 * (aA - aB) / ((r1.Mass + r2.Mass)) * r2.Mass) * col1.normal;
       //col2.ReflectionVector = (r2.Velocity + (2 * (aA - aB)) / col2.normal * ((r1.Mass + r2.Mass)) * r1.Mass);
       //
       ////update the end points of where the two circles will end up
       //col1.PointEnd = col1.interPoint + col1.ReflectionVector * (1.0f - col1.interTime);
       //col2.PointEnd = col2.interPoint + col2.ReflectionVector * (1.0f - col2.interTime);
   }
}