#include "CollisionSystem.h"
#include "CollisionResponseCallbacks.h"
#include "Collision.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
	void CollisionSystem::update()
	{
	  CollisionIntersectionCheck();
	  CollisionHandling();
	  CollisionResolution();

	}

	void CollisionSystem::late_update()
	{
	}

	void CollisionSystem::Init()
	{
	}

	void CollisionSystem::CollisionIntersectionCheck()
	{
		PreviousPair.swap(CurrentPair);
		CurrentPair.clear();
		bool Intersected = false;
		//Collision Intersection
		em.for_each([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
		{
			if (c1.isCollideable)
			{
				c1.center = t1.position;
				c1.size = t1.scale;
				Vector2 OldPos = t1.position;
				em.for_each([&](EntityID id2, Transform& t2, Collider& c2)
				{
					if(c2.isCollideable)
					{
						if (id1.index != id2.index)
						{
							c2.center = t2.position;
							c2.size = t2.scale;
							if (CollisionIntersection_Main(c1, c2))
							{
								DeltaEngine_CORE_TRACE("COLLISION");
								CurrentPair.push_back({ id1,id2 });
							}
							else
							{
								DeltaEngine_CORE_TRACE("NOT_COLLISION");
							}
							t1.position = OldPos;
						}
					}
				});
			}
		});

	}

	void CollisionSystem::CollisionHandling()
	{
		bool Handled = false;
		if (!CurrentPair.empty())
		{
			for (auto it1 = CurrentPair.begin(); it1 != CurrentPair.end(); it1++)
			{
				for (auto it2 = PreviousPair.begin(); it2 != PreviousPair.end(); it2++)
				{
					if (it1->first.index == it2->first.index && it1->second.index == it2->second.index)
					{
						collision_handler.OnStay(it1->first);
						collision_handler.OnStay(it1->second);
						Handled = true;
						DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON STAY");
					}
				}

				if (!Handled)
				{
					collision_handler.OnEnter(it1->first);
					collision_handler.OnEnter(it1->second);
					DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON ENTER");
				}
			}
		}



		if (!Handled && !PreviousPair.empty())
		{
			//handle previous pair exit
			for (auto it3 = PreviousPair.begin(); it3 != PreviousPair.end(); it3++)
			{
				collision_handler.OnExit(it3->first);
				collision_handler.OnExit(it3->second);
				DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON EXIT");
			}
		}

	}

	void CollisionSystem::CollisionResolution()
	{
		int counter = 0;
		for (auto it1 = CurrentPair.begin(); it1 != CurrentPair.end(); it1++)
		{
			em.for_each([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
			{
				if (it1->first.index == id1.index || it1->second.index == id1.index)
				{
					em.for_each([&](EntityID id2, RigidBody& r2, Transform& t2, Collider& c2)
					{
						if (it1->first.index == id2.index || it1->second.index == id2.index)
						{
							//float CollidingObjectsMass = r1.Mass + r2.Mass;
							//Vector2 CollidingObjectsVelocity = r1.Velocity + r2.Velocity;
							//r1.Velocity += (CollidingObjectsVelocity * CollidingObjectsMass) / (r1.Mass + CollidingObjectsMass);
							//r2.Velocity += (CollidingObjectsVelocity * CollidingObjectsMass) / (r2.Mass + CollidingObjectsMass);
							t1.position = t1.old_position;
							t2.position = t2.old_position;
							r1.Velocity -= r2.Velocity * 0.5f;
							r2.Velocity -= r1.Velocity * 0.5f;

							DeltaEngine_CORE_TRACE("RESOLVING COLLISION");
						}
					});
				}
				counter++;
			});

		}

	}

}

