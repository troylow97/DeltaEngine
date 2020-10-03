#include "CollisionSystem.h"
#include "RigidBody.h"
#include "Core/Math/Transform.h"
#include "CollisionResponseCallbacks.h"

namespace DeltaEngine
{
	void CollisionSystem::update()
	{
	  em.for_each(e_query, [&](EntityID id, RigidBody& r, Transform& t, Collider& c)
	  {

	  });
 

	  CollisionIntersectionCheck();
	  CollisionResolution();
	  CollisionHandling();


	}

	void CollisionSystem::late_update()
	{
		// Codes
	}

	void CollisionSystem::Init()
	{
		//CollisionSystem* collision_sys = ecs->register_system<CollisionSystem>();
		//{
		//	DeltaEngine::BitSignature signature;
		//	signature.set(ecs->get_component_type<RigidBody>());
		//	signature.set(ecs->get_component_type<Transform>());
		//	signature.set(ecs->get_component_type<Collider*>());
		//	ecs->set_system_signature<CollisionSystem>(signature);
		//
		//}
	}

	void CollisionSystem::CollisionIntersectionCheck()
	{
		PreviousPair.swap(CurrentPair);
		CurrentPair.clear();
		bool Intersected = false;

		//Collision Intersection
		em.for_each(e_query, [&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
			{
				Vector2 OldPos = t1.position;
				c1.center = c1.center + r1.Velocity;
				//c1.OnUpdate(id1);
				em.for_each(e_query, [&](EntityID id2, RigidBody& r2, Transform& t2, Collider* c2)
					{
						ColliderType collider_type = c2->GetType();
						switch (collider_type)
						{
						case ColliderType::BOX:
						{
							Intersected = c1.Intersection(dynamic_cast<BoxCollider*>(c2));
						}
						case ColliderType::CIRCLE:
						{
							Intersected = c1.Intersection(dynamic_cast<CircleCollider*>(c2));
						}
						case ColliderType::LINE:
						{
							Intersected = c1.Intersection(dynamic_cast<LineCollider*>(c2));
						}
						case ColliderType::RAY:
						{
							Intersected = c1.Intersection(dynamic_cast<RayCollider*>(c2));
						}
						}
						if (Intersected)
						{
							CollisionPair pair{ id1,id2 };
							CurrentPair.push_back(pair);
						}
						t1.position = OldPos;
					});
			});
	}

	void CollisionSystem::CollisionHandling()
	{
		//bool Handled = false;
		//
		//for (auto it1 = CurrentPair.begin(); it1 != CurrentPair.end();)
		//{
		//	Handled = false;
		//	for (auto it2 = PreviousPair.begin(); it2 != PreviousPair.end(); )
		//	{
		//		if (*it1 == *it2)
		//		{
		//			//handle currentpairs stay
		//			//collison_handlers.CollisionHandling[pair1].
		//			collision_handler.OnStay(it1->first);
		//			collision_handler.OnStay(it1->second);
		//			Handled = true;
		//		}
		//	}
		//
		//	if (!Handled)
		//	{
		//		collision_handler.OnEnter(it1->first);
		//		collision_handler.OnEnter(it1->second);
		//	}
		//}
		//
		//if (!Handled)
		//{
		//	//handle previous pair exit
		//	for (auto it3 = PreviousPair.begin(); it3 != PreviousPair.end(); )
		//	{
		//		collision_handler.OnExit(it3->first);
		//		collision_handler.OnExit(it3->second);
		//	}
		//}
	}

	void CollisionSystem::CollisionResolution()
	{
		//for (auto it1 = CurrentPair.begin(); it1 != CurrentPair.end();)
		//{
		//	//em.for_each(query, [&]() EntityID id, Transform& t, Collider& c
		//	//{
		//	//	c.centre = t.position;
		//	//});
		//	//AABB_CollisionResponse(*collider_1, *collider_2, rigid_1.Velocity, rigid_2.Velocity);
		//}

	}

}

