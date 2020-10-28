#include "CollisionResponseCallbacks.h"
#include "Core/Math/Math.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Collision.h"
namespace DeltaEngine
{
	void CollisionResponse_AABBvsAABB(Collider& obj1, RigidBody& r1,Collider& obj2,RigidBody& r2,Manifold& m)
	{
		float e = Math::MathMin(r1.Restitution, r2.Restitution);
		//float accumulated_inverse_mass = 1 / r1.Mass + 1 / r2.Mass;

		//std::cout << "accumulated_inverse_mass IS " << accumulated_inverse_mass << std::endl;
		r1.Velocity += (m.normal * m.penetration) * e;
		r2.Velocity -= (m.normal * m.penetration) * e;


	}

   void CollisionResponse_CirclevsCircle(Collider& col1,RigidBody& r1,Transform& t1,Collider& col2, RigidBody& r2,Transform& t2,Manifold& m)
   {

	   //get the direction of reflection using dot product	
		float a1{ Vector2DotProduct(r1.Velocity, m.normal) };
		float a2{ Vector2DotProduct(r2.Velocity, m.normal) };
		
		//calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
		Vector2 reflectedVectorA = m.normal * (r1.Velocity - (2 * (a1 - a2) / (r1.Mass + r2.Mass)) * r2.Mass);
		Vector2 reflectedVectorB = m.normal * (r2.Velocity + (2 * (a1 - a2) / (r1.Mass + r2.Mass)) * r1.Mass);

		t1.position = col1.interPoint + reflectedVectorA * (1.0f - m.interTime);
		t2.position = col2.interPoint + reflectedVectorB * (1.0f - m.interTime);
		
		col1.collided_spot = { -10,-10 };
		col2.collided_spot = { -10,-10 };

		float speed = Vector2Length(reflectedVectorA) / env.pClock->DeltaTime();
		reflectedVectorA = Normalise(reflectedVectorA);

		r1.Velocity = reflectedVectorA * speed;

		speed = Vector2Length(reflectedVectorB) / env.pClock->DeltaTime();
		reflectedVectorB = Normalise(reflectedVectorB);

		r2.Velocity = reflectedVectorB * speed;
   }

   void CollisionResponse_RectvsCircle(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m)
   {
	   t1.position += r1.ReflectedVector;
	   t2.position += r2.ReflectedVector;
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
		   CollisionResponse_AABBvsAABB(col1, r1,col2, r2,m);
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
		   CollisionResponse_RectvsCircle(col1, r1, t1, col2, r2, t2, m);
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

   Vector2 AABBvsAABBoverlap(const Collider& obj1, const Collider& obj2)
   {
	   float TotalWidth = obj1.size.x + obj2.size.x;
	   float ActualWidth = abs(obj2.size.x - obj1.size.x);

	   float TotalHeight = obj1.size.y + obj2.size.y;
	   float ActualHeight= abs(obj2.size.y - obj1.size.y);

	   Vector2 overlap = { 0,0 };

	   if (ActualWidth < TotalWidth)
	   {
		   overlap.x = TotalWidth - ActualWidth;
	   }

	   if (ActualHeight < TotalHeight)
	   {
		   overlap.y = TotalHeight - ActualHeight;
	   }

	   return overlap;

   }
}