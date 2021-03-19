/**********************************************************************************
* \file   CollisionSystem.cpp
* \brief  The file contains logic for collision system
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "CollisionSystem.h"
#include "Collision.h"
#include "Core/Math/Math.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/EngineClock.h"
#include "Manifold.h"
#include "Core/Debugging/Profiler/Profiler.h"


namespace DeltaEngine
{
  CollisionHandler CollisionSystem::collision_handler;

  void CollisionSystem::Update()
  {
    for (size_t step = 0; step < env.pClock->Timesteps(); ++step)
    {
      CollisionIntersectionCheck();
      CollisionHandling();
      CollisionResolution();
    }
    Profiler::Instance().Record("Collision System");
  }

  void CollisionSystem::LateUpdate()
  {
  }

  void CollisionSystem::Initialize()
  {
  }

  void CollisionSystem::CollisionIntersectionCheck()
  {
    old_manifold_vector.swap(current_manifold_vector);
    current_manifold_vector.clear();

    //Collision Intersection
    em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
    {
      //if (c1.CollisionLayerCheck != 0)
      {
        c1.isCollidingOnFloor = false;
        c1.center = c1.offset + t1.position;
        em.ForEach([&](EntityID id2, RigidBody& r2, Transform& t2, Collider& c2)
        {
          if (/*c2.CollisionLayerCheck != 0 && */id2.index != id1.index)
          {
            c2.isCollidingOnFloor = false;
            c2.center = c2.offset + t2.position;
            Manifold m;
            if (r1.isMoveable || r2.isMoveable)
            {
              if (c1.CollisionLayerID & c2.CollisionLayerCheck)
              {
                if (CollisionIntersection_RectRect(c1.center, c1.size * t1.scale, r1.Velocity, c2.center,
                                                   c2.size * t2.scale, r2.Velocity))
                {
                  bool already_added = false;
                  //Check if there was already collision between the two 
                  for (auto it1 = current_manifold_vector.begin(); it1 != current_manifold_vector.end(); ++it1)
                  {
                    if ((it1->id1.index == id1.index && it1->id2.index == id2.index) || (it1->id1.index == id2.index &&
                      it1->id2.index == id1.index))
                    {
                      already_added = true;
                      break;
                    }
                  }

                  if (!already_added)
                  {
                    c1.isCollidingOnFloor = false;
                    c2.isCollidingOnFloor = false;
                    if (AABBvsAABB_Manifold(c1, t1.scale, c2, t2.scale, m))
                      current_manifold_vector.push_back({m, id1, id2});
                  }
                }
              }
            }
          }
        });
      }
    });
  }

  void CollisionSystem::CollisionHandling()
  {
    bool Handled = false;
    if (!current_manifold_vector.empty()) //if vector is full
    {
      for (auto it1 = current_manifold_vector.begin(); it1 !=
           current_manifold_vector.end(); ++it1)
      {
        for (auto it2 = old_manifold_vector.begin(); it2 != old_manifold_vector.
             end(); ++it2)
        {
          if (it1->id1.index == it2->id1.index && it1->id2.index == it2->id2.index)
          {
            collision_handler.OnStay(it1->id1, it1->id2);
            Handled = true;
            it1->handled = true;
            it2->handled = true;
          }
        }

        if (!it1->handled)
        {
          collision_handler.OnEnter(it1->id1, it1->id2);
          it1->handled = true;
        }
      }
    }


    if (!old_manifold_vector.empty())
    {
      //handle previous pair exit
      for (auto it3 = old_manifold_vector.begin(); it3 != old_manifold_vector.end(); ++it3)
      {
      	if(!it3->handled)
			collision_handler.OnExit(it3->id1, it3->id2);
      }
    }
  }

  void CollisionSystem::CollisionResolution()
  {
    //Resolve LowestPenetrationFirst
    //std::sort(current_manifold_vector.begin(), current_manifold_vector.end(), [](const CollisionPairInfo& a, const CollisionPairInfo& b)
    //{
    //        return a.m.penetration < b.m.penetration;
    //});

    //for (int i = 0; i < 2; ++i)
    {
      for (auto it1 = current_manifold_vector.begin(); it1 != current_manifold_vector.end(); ++it1)
      {
        Collider& c1 = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(it1->id1);
        Collider& c2 = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(it1->id2);
      	
        if (!it1->ignore && (!c1.isTrigger && !c2.isTrigger))
        {
          RigidBody& r1 = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(it1->id1);
          RigidBody& r2 = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(it1->id2);
          Transform& t1 = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(it1->id1);
          Transform& t2 = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(it1->id2);

          //Platform logic
          if ((c1.isPlatform || c2.isPlatform) && (em.HasComponent<Player>(it1->id1) || em.HasComponent<Player
          >(it1->id2)))
          {
            EntityID player;
            EntityID platform;
            if (em.HasComponent<Player>(it1->id1))
            {
              player = it1->id1;
              platform = it1->id2;
            }
            else
            {
              player = it1->id2;
              platform = it1->id1;
            }


          	
            if ((em.GetComponent<Transform>(player).position.y - 0.25f) < em.GetComponent<Transform>(platform).position.y)
            {
              it1->ignore = true;
              em.GetComponent<RigidBody>(player).AccumulatedForce += {0, 500};
              continue;
            }
          }

          //Standard Collision Response
          if ((AABBvsAABB_Manifold(c1, t1.scale, c2, t2.scale, it1->m) && it1->m.penetration > 0.001f))
          {
            CollisionResponse(c1, r1, c2, r2, it1->m);

            if (r1.isMoveable)
            {
              t1.position = r1.PointEnd - c1.offset;
              c1.center = c1.offset + t1.position;
            }
            if (r2.isMoveable)
            {
              t2.position = r2.PointEnd - c2.offset;
              c2.center = c2.offset + t2.position;
            }
          }
        }
      }
    }


    //for (auto it1 = current_manifold_vector.begin(); it1 != current_manifold_vector.end(); it1++)
    //{
    //    RigidBody& r1 = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(it1->id1);
    //    RigidBody& r2 = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(it1->id2);
    //    Transform& t1 = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(it1->id1);
    //    Transform& t2 = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(it1->id2);
    //    Collider& c1 = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(it1->id1);
    //    Collider& c2 = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(it1->id2);
    //
    //    if (!c1.isTrigger && !c2.isTrigger && it1->m.penetration > 0.01f)
    //    {
    //        t1.position += it1->m.normal * 0.01f;
    //        t1.position -= it1->m.normal * 0.01f;
    //    }
    //
    //}
  }
}
