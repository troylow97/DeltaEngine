#include "CollisionResponseCallbacks.h"
#include "Core/Math/Math.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Collision.h"
namespace DeltaEngine
{
	void CollisionResponse_AABBvsAABB(Collider& obj1, RigidBody& r1,Collider& obj2,RigidBody& r2)
	{
		Vector2 obj1Top, obj1Right, obj1Left, obj1Bot, obj1TopLeft, obj1TopRight, obj1BotLeft, obj1BotRight;
		Vector2 obj2Top, obj2Right, obj2Left, obj2Bot,obj2TopLeft,obj2TopRight,obj2BotLeft,obj2BotRight;
		float obj1HalfWidth, obj1HalfHeight, obj2HalfWidth, obj2HalfHeight;
		obj1HalfWidth = obj1.size.x / 2;
		obj1HalfHeight = obj1.size.y / 2;
		obj2HalfWidth = obj2.size.x / 2;
		obj2HalfHeight = obj2.size.y / 2;
		obj1Top = { obj1.center.x,obj1.center.y + obj1HalfHeight };
		obj1Bot = { obj1.center.x,obj1.center.y - obj1HalfHeight };
		obj1Right = { obj1.center.x + obj1HalfWidth, obj1.center.y };
		obj1Left = { obj1.center.x - obj1HalfWidth, obj1.center.y };

		obj2Top = { obj2.center.x,obj2.center.y + obj2HalfHeight };
		obj2Bot = { obj2.center.x,obj2.center.y - obj2HalfHeight };
		obj2Right = { obj2.center.x + obj2HalfWidth, obj2.center.y };
		obj2Left = { obj2.center.x - obj2HalfWidth, obj2.center.y };

		obj2TopLeft = { obj2.center.x - obj2HalfWidth,obj2.center.y + obj2HalfHeight };
		obj2BotLeft = { obj2.center.x - obj2HalfWidth,obj2.center.y - obj2HalfHeight };
		obj2TopRight = { obj2.center.x + obj2HalfWidth, obj2.center.y + obj2HalfHeight };
		obj2BotRight = { obj2.center.x - obj2HalfWidth, obj2.center.y - obj2HalfHeight };
		
		obj1TopLeft = { obj1.center.x - obj1HalfWidth,obj1.center.y + obj1HalfHeight };
		obj1BotLeft = { obj1.center.x - obj1HalfWidth,obj1.center.y - obj1HalfHeight };
		obj1TopRight = { obj1.center.x + obj1HalfWidth, obj1.center.y + obj1HalfHeight };
		obj1BotRight = { obj1.center.x - obj1HalfWidth, obj1.center.y - obj1HalfHeight };

		if (CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Top))
		{
			r1.Velocity = { 0, 1 };
			r2.Velocity = { 0, -1 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Bot))
		{
			r1.Velocity = { 0, -1 };
			r2.Velocity = { 0, 1 };
			return;
		}		
		else if (CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Right))
		{
			r1.Velocity = { 1,0 };
			r2.Velocity = { -1,0 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Left))
		{
			r1.Velocity = { -1,0 };
			r2.Velocity = { 1, 0 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2TopLeft) && CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Bot)
				|| CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2TopRight) && CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Bot))
		{
			r1.Velocity = { 0, 1 };
			r2.Velocity = { 0, -1 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2TopLeft) && CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Right)
				|| CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2BotLeft) && CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Right))
		{
			r1.Velocity = { -1, 0 };
			r2.Velocity = { 1,0 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2BotLeft) && CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Top)
				|| CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2BotRight) && CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Top))
		{
			r1.Velocity = { 0, -1};
			r2.Velocity = { 0,1 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2TopRight) && CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Left)
				|| CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2BotRight) && CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Left))
		{
			r1.Velocity = { 1, 0 };
			r2.Velocity = { -1,0 };
			return;
		}

		if (CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Top))
		{
			r2.Velocity = { 0, 1 };
			r1.Velocity = { 0, -1 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Bot))
		{
			r2.Velocity = { 0, -1 };
			r1.Velocity = { 0, 1 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Right))
		{
			r2.Velocity = { 1, 0 };
			r1.Velocity = { -1, 0 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1Left))
		{
			r2.Velocity = { -1, 0 };
			r1.Velocity = { 1, 0 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1TopLeft) && CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Bot)
			|| CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1TopRight) && CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Bot))
		{
			r2.Velocity = { 0, 1 };
			r1.Velocity = { 0, -1 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1TopLeft) && CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Right)
			|| CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1BotLeft) && CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Right))
		{
			r2.Velocity = { -1,0 };
			r1.Velocity = { 1,0 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1BotLeft) && CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Top)
			|| CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1BotRight) && CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Top))
		{
			r2.Velocity = { 0,-1 };
			r1.Velocity = { 0,1 };
			return;
		}
		else if (CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1TopRight) && CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Left)
			|| CollisionIntersection_RectPoint(obj2.center, obj2.size, obj1BotRight) && CollisionIntersection_RectPoint(obj1.center, obj1.size, obj2Left))
		{
			r1.Velocity = { -1,0 };
			r1.Velocity = { -1,0 };
			return;
		}
		//Vector2 zero_vec = { 0,0 };
		//Vector2 normalised_r1 = Normalise(r1.Velocity);
		//Vector2 normalised_r2 = Normalise(r2.Velocity);
		////float r1Magnitude = (r1.Velocity.x * r1.Velocity.x + r1.Velocity.y * r1.Velocity.y);
		////float r2Magnitude = (r2.Velocity.x * r2.Velocity.x + r2.Velocity.y * r2.Velocity.y);
		//
		//if (normalised_r1 != zero_vec)
		//{
		//	r1.Velocity = -r1.Direction * 2.0f; //multiply this for knockback, bugged in one case
		//	r2.Velocity = r1.Direction;
		//}
		//else if (normalised_r2 != zero_vec)
		//{
		//	r2.Velocity = -r2.Direction * 2.0f; //multiply this for knockback, bugged in one case
		//	r1.Velocity = r2.Direction;
		//}
		//else
		//{
		//	r1.Velocity = -r1.Direction * 2.0f;
		//	r2.Velocity = -r2.Direction * 2.0f;
		//}


	}

   void CollisionResponse_CirclevsCircle(Collider& col1,RigidBody& r1,Transform& t1,Collider& col2, RigidBody& r2,Transform& t2,Manifold& m)
   {
				// Calculate a1 & a2 magnitude
				//float a1{ Vector2DotProduct(r1.Velocity, m.normal) };
				//float a2{ Vector2DotProduct(r2.Velocity, m.normal) };
				//
				//// Calculate P
				//float p{ (2 * (a1 - a2) / (r1.Mass + r2.Mass)) };
				//
				//// Calculate reflected vector
				//Vector2 reflectedVectorA = r1.Velocity - (m.normal * p * r2.Mass);
				//Vector2 reflectedVectorB = r2.Velocity -(m.normal * p * r1.Mass);
				//
				//// Calculate final position
				//r1.Velocity = col1.interPoint + reflectedVectorA * (1.0f - m.interTime);
				//r2.Velocity = col2.interPoint + reflectedVectorB * (1.0f - m.interTime);
	  //m.normal = col1.interPoint - col2.interPoint;
	  //Normalise(m.normal);
	  //
      ////get the direction of reflection using dot product
      //float aA = Vector2DotProduct(r1.Velocity, m.normal);
      //float aB = Vector2DotProduct(r2.Velocity, m.normal);
	  //
      ////calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
	  //r1.ReflectedVector = r1.Velocity - (2 * (aA - aB) / (r1.Mass + r2.Mass));
	  //r1.ReflectedVector = r1.ReflectedVector * r2.Mass * m.normal;
	  //
	  //r2.ReflectedVector = r2.Velocity - (2 * (aA - aB) / (r1.Mass + r2.Mass));
	  //r2.ReflectedVector = r2.ReflectedVector * r1.Mass * m.normal;
      ////update the end points of where the two circles will end up
      //t1.end_point =  r1.ReflectedVector * (1.0f - m.interTime) + col1.interPoint;
      //t2.end_point = r2.ReflectedVector * (1.0f - m.interTime) + col2.interPoint;
	  //
	  ////r1.Movespeed = Vector2Length(r1.ReflectedVector) / env.pClock->DeltaTime();
	  //Normalise(r1.ReflectedVector);//A: new speed direction
	  //
	  //r1.Velocity = r1.ReflectedVector * env.pClock->DeltaTime();
	  //
	 ////r2.Movespeed = Vector2Length(r2.ReflectedVector) / env.pClock->DeltaTime();//B: new speed
	  //Normalise(r2.ReflectedVector);//B: new speed direction
	  //
	  //r2.Velocity = r2.ReflectedVector * env.pClock->DeltaTime();
   }

   void CollisionResponse_RectvsCircle(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m)
   {
	   r1.Velocity -= r1.ReflectedVector; //r1 is the Rect
	   r2.Velocity = -r2.ReflectedVector;
   }

   void CollisionResponse_Main(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m)
   {
	   ColliderType type1 = col1.type;

	   switch (type1)
	   {
	   case ColliderType::BOX:
		   CollisionResponse_Box(col1, r1, t1, col2, r2, t2, m);
		   return;
	   case ColliderType::CIRCLE:
		   CollisionResponse_Circle(col1, r1, t1, col2, r2, t2, m);
		   return;
	   case ColliderType::LINE:
		   return;
	   case ColliderType::RAY:
		   return;
	   default:
		   CollisionResponse_Box(col1,r1,t1,col2,r2,t2,m);
		   return;
	   }
   }

   void CollisionResponse_Box(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m)
   {
	   ColliderType type2 = col2.type;

	   switch (type2)
	   {
	   case ColliderType::BOX:
		   CollisionResponse_AABBvsAABB(col1, r1,col2, r2);
		   return;
	   case ColliderType::CIRCLE:
		   CollisionResponse_RectvsCircle(col1, r1, t1, col2, r2, t2, m);
		   return;
	   case ColliderType::LINE:
		   return;
	   case ColliderType::RAY:
		   return;
	   default:
		   return;
	   }
   }

   void CollisionResponse_Circle(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m)
   {
	   ColliderType type2 = col2.type;

	   switch (type2)
	   {
	   case ColliderType::BOX:
		   return;
	   case ColliderType::CIRCLE:
		   CollisionResponse_CirclevsCircle(col1, r1, t1, col2, r2, t2, m);
		   return;
	   case ColliderType::LINE:
		   return;
	   case ColliderType::RAY:
		   return;
	   default:
		   return;
	   }
   }
}