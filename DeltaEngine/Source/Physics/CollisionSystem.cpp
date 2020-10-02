#include "CollisionSystem.h"
#include "RigidBody.h"
#include "Core/Math/Transform.h"
#include "CollisionResponseCallbacks.h"

namespace DeltaEngine
{
	void CollisionSystem::Init()
	{
		CollisionSystem* collision_sys = ecs->register_system<CollisionSystem>();
		{
			DeltaEngine::BitSignature signature;
			signature.set(ecs->get_component_type<RigidBody>());
			signature.set(ecs->get_component_type<Transform>());
			signature.set(ecs->get_component_type<Collider*>());
			ecs->set_system_signature<CollisionSystem>(signature);

		}
	}

	void CollisionSystem::CollisionIntersectionCheck()
	{
		PreviousPair.swap(CurrentPair);
		CurrentPair.clear();
		bool Intersected = false;
		//Collision Intersection
		for (DeltaEngine::Entity ref1 : entities)
		{
			Collider* collider_1 = ecs->get_component<Collider*>(ref1);
			Vector2 OldPos = ecs->get_component<Transform>(ref1).position;
			ecs->get_component<Collider*>(ref1)->center = ecs->get_component<Transform>(ref1).position + ecs->get_component<RigidBody>(ref1).Velocity;
			ecs->get_component<Collider*>(ref1)->OnUpdate(ref1);
			for (DeltaEngine::Entity ref2 : entities)
			{
				Collider* collider_2 = ecs->get_component<Collider*>(ref2);
				ColliderType collider_type = collider_2->GetType();
				switch (collider_type)
				{
					case ColliderType::BOX:
					{
						Intersected = collider_1->Intersection(dynamic_cast<BoxCollider*>(collider_2));
					}
					case ColliderType::CIRCLE:
					{
						Intersected = collider_1->Intersection(dynamic_cast<CircleCollider*>(collider_2));
					}
					case ColliderType::LINE:
					{
						Intersected = collider_1->Intersection(dynamic_cast<LineCollider*>(collider_2));
					}
					case ColliderType::RAY:
					{
						Intersected = collider_1->Intersection(dynamic_cast<RayCollider*>(collider_2));
					}
				}
				CollisionPair pair{ ref1,ref2 };
				CurrentPair.push_back(pair);

			}
			ecs->get_component<Collider*>(ref1)->center = OldPos;
			ecs->get_component<Collider*>(ref1)->OnUpdate(ref1);
		}

	}

	void CollisionSystem::CollisionHandling()
	{
		bool Handled = false;

		for (auto it1 = CurrentPair.begin(); it1 != CurrentPair.end();)
		{
			Handled = false;
			for (auto it2 = PreviousPair.begin(); it2 != PreviousPair.end(); )
			{
				if (*it1 == *it2)
				{
					//handle currentpairs stay
					//collison_handlers.CollisionHandling[pair1].
					collision_handler.OnStay(it1->first);
					collision_handler.OnStay(it1->second);
					Handled = true;
				}
			}

			if (!Handled)
			{
				collision_handler.OnEnter(it1->first);
				collision_handler.OnEnter(it1->second);
			}
		}

		if (!Handled)
		{
			//handle previous pair exit
			for (auto it3 = PreviousPair.begin(); it3 != PreviousPair.end(); )
			{
				collision_handler.OnExit(it3->first);
				collision_handler.OnExit(it3->second);
			}
		}
	}

	void CollisionSystem::CollisionResolution()
	{
		for (auto it1 = CurrentPair.begin(); it1 != CurrentPair.end();)
		{
			RigidBody& rigid_1 = ecs->get_component<RigidBody>(it1->first);
			Transform& trans_1 = ecs->get_component<Transform>(it1->first);
			Collider* collider_1 = ecs->get_component<Collider*>(it1->first);
			RigidBody& rigid_2 = ecs->get_component<RigidBody>(it1->second);
			Transform& trans_2 = ecs->get_component<Transform>(it1->second);
			Collider* collider_2 = ecs->get_component<Collider*>(it1->second);
			AABB_CollisionResponse(*collider_1, *collider_2, rigid_1.Velocity, rigid_2.Velocity);
		}
	}

	void CollisionSystem::Update()
	{
		CollisionIntersectionCheck();
		CollisionResolution();
		CollisionHandling();

	}

}

