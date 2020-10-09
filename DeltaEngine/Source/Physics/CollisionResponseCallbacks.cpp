#include "CollisionResponseCallbacks.h"
#include "Core/Math/Math.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"

namespace DeltaEngine
{
	void CollisionResponse_AABBvsAABB(Collider& obj1, RigidBody& r1,Collider& obj2,RigidBody& r2)
	{
		Vector2 zero_vec = { 0,0 };
		Vector2 normalised_r1 = Normalise(r1.Velocity);
		Vector2 normalised_r2 = Normalise(r2.Velocity);
		//float r1Magnitude = (r1.Velocity.x * r1.Velocity.x + r1.Velocity.y * r1.Velocity.y);
		//float r2Magnitude = (r2.Velocity.x * r2.Velocity.x + r2.Velocity.y * r2.Velocity.y);

		if (normalised_r1 != zero_vec)
		{
			r1.Velocity = -r1.Direction * 2.0f; //multiply this for knockback, bugged in one case
			r2.Velocity = r1.Direction;
		}
		else if (normalised_r2 != zero_vec)
		{
			r2.Velocity = -r2.Direction * 2.0f; //multiply this for knockback, bugged in one case
			r1.Velocity = r2.Direction;
		}
		else
		{
			r1.Velocity = -r1.Direction * 2.0f;
			r2.Velocity = -r2.Direction * 2.0f;
		}


	}

   void CollisionResponse_CirclevsCircle(Collider& col1,RigidBody& r1,Transform& t1,Collider& col2, RigidBody& r2,Transform& t2,Manifold& m)
   {
	 //  m.normal = col1.interPoint - col2.interPoint;
	 //  Normalise(m.normal);
	 //
     //  //get the direction of reflection using dot product
     //  float aA = Vector2DotProduct(r1.Velocity, m.normal);
     //  float aB = Vector2DotProduct(r2.Velocity, m.normal);
	 //
     //  //calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
	 //  r1.ReflectedVector = r1.Velocity - (2 * (aA - aB) / (r1.Mass + r2.Mass));
	 //  r1.ReflectedVector = r1.ReflectedVector * r2.Mass * m.normal;
	 //
	 //  r2.ReflectedVector = r2.Velocity - (2 * (aA - aB) / (r1.Mass + r2.Mass));
	 //  r2.ReflectedVector = r2.ReflectedVector * r1.Mass * m.normal;
     //  //update the end points of where the two circles will end up
     //  t1.end_point =  r1.ReflectedVector * (1.0f - m.interTime) + col1.interPoint;
     //  t2.end_point = r2.ReflectedVector * (1.0f - m.interTime) + col2.interPoint;
	 //
	 //  //r1.Movespeed = Vector2Length(r1.ReflectedVector) / env.pClock->DeltaTime();
	 //  Normalise(r1.ReflectedVector);//A: new speed direction
	 //
	 //  r1.Velocity = r1.ReflectedVector * env.pClock->DeltaTime();
	 //
	 // //r2.Movespeed = Vector2Length(r2.ReflectedVector) / env.pClock->DeltaTime();//B: new speed
	 //  Normalise(r2.ReflectedVector);//B: new speed direction
	 //
	 //  r2.Velocity = r2.ReflectedVector * env.pClock->DeltaTime();
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