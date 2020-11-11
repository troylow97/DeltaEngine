#include "CollisionSystem.h"
#include "Collision.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/Math/Math.h"
#include <cmath>
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Components/Character.h"
#include "Manifold.h"

namespace DeltaEngine
{
void CollisionSystem::Update()
{
    for (size_t step = 0; step < env.pClock->Timesteps(); ++step)
    {
        CollisionIntersectionCheck();
        CollisionHandling();
        CollisionResolution();
    }
}

void CollisionSystem::LateUpdate()
{}

void CollisionSystem::Init()
{}

void CollisionSystem::CollisionIntersectionCheck()
{
  old_manifold_vector.swap( current_manifold_vector );
  current_manifold_vector.clear();

  //Collision Intersection
  em.ForEach( [&]( EntityID id1, RigidBody &r1, Transform &t1, Collider &c1 )
  {
    if ( c1.isCollideable )
    {
      c1.isCollidingOnFloor = false;
      c1.center = t1.position;
      c1.size = t1.scale;
      em.ForEach( [&]( EntityID id2, RigidBody &r2, Transform &t2, Collider &c2 )
      {
        if (c2.isCollideable)
        {
          if ( id1.index != id2.index )
          {
            c2.isCollidingOnFloor = false;
            c2.center = t2.position;
            c2.size = t2.scale;
            Manifold m;
            if (CollisionIntersection_Main(c1, r1, c2, r2, m))
            {
                bool already_added = false;
                //Check if there was already collision between the two 
                for (auto it1 = current_manifold_vector.begin(); it1 != current_manifold_vector.end(); it1++)
                {
                    if ((it1->id1.index == id1.index && it1->id2.index == id2.index) || (it1->id1.index == id2.index && it1->id2.index == id1.index))
                    {
                        already_added = true;
                        break;
                    }
                }



                if (!already_added)
                {
                    AABBvsAABB_Manifold(c1, c2, m);
                    current_manifold_vector.push_back({ m,id1,id2});
                }

            }

          }

        }
      } );
    }
  } );

}

void CollisionSystem::CollisionHandling()
{
  bool Handled = false;
  if ( !current_manifold_vector.empty() )
  {
    for ( std::vector<CollisionPairInfo>::iterator it1 = current_manifold_vector.begin(); it1 != current_manifold_vector.end(); it1++ )
    {
      for ( std::vector<CollisionPairInfo>::iterator it2 = old_manifold_vector.begin(); it2 != old_manifold_vector.end(); it2++ )
      {
        if ( it1->id1.index == it2->id1.index && it1->id2.index == it2->id2.index )
        {
          collision_handler.OnStay( it1->id1 );
          collision_handler.OnStay( it1->id2 );
          Handled = true;
          //DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON STAY");
        }
      }

      if ( !Handled )
      {
        collision_handler.OnEnter( it1->id1);
        collision_handler.OnEnter( it1->id2);
        //DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON ENTER");
      }
    }
  }



  if ( !Handled && !old_manifold_vector.empty() )
  {
    //handle previous pair exit
    for ( auto it3 = old_manifold_vector.begin(); it3 != old_manifold_vector.end(); it3++ )
    {
      collision_handler.OnExit( it3->id1 );
      collision_handler.OnExit( it3->id2 );
      //DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON EXIT");
    }
  }

}

void CollisionSystem::CollisionResolution()
{
   //Resolve Lowest Contact First
   std::sort(current_manifold_vector.begin(), current_manifold_vector.end(), [](const CollisionPairInfo& a, const CollisionPairInfo& b)
   {
        return a.m.ContactPoint.y < b.m.ContactPoint.y;
   });
   

    for ( auto it1 = current_manifold_vector.begin(); it1 != current_manifold_vector.end(); it1++ )
    {
      em.ForEach( [&]( EntityID id1, RigidBody &r1, Transform &t1, Collider &c1 )
      {
        em.ForEach( [&]( EntityID id2, RigidBody &r2, Transform &t2, Collider &c2 )
        {
          if ( it1->id1.index == id1.index && it1->id2.index == id2.index && !c1.isTrigger && !c2.isTrigger)
          {
              CollisionResponseMain(c1, r1, c2, r2, it1->m);

              if (r1.isMoveable)
              {
                  c1.center = r1.PointEnd;
              }
              if (r2.isMoveable)
              {
                  c2.center = r2.PointEnd;
              }
          }
    
        } );
      } );
    }

    for (auto it1 = current_manifold_vector.begin(); it1 != current_manifold_vector.end(); it1++)
    {
        em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
            {
                em.ForEach([&](EntityID id2, RigidBody& r2, Transform& t2, Collider& c2)
                    {
                        if (it1->id1.index == id1.index && it1->id2.index == id2.index && !c1.isTrigger && !c2.isTrigger)
                        {
                            if (r1.isMoveable)
                            {
                                t1.position = r1.PointEnd;
                                c1.center = r1.PointEnd;
                            }
                            if (r2.isMoveable)
                            {
                                t2.position = r2.PointEnd;
                                c2.center = r2.PointEnd;
                            }
                        }

                    });
            });
    }

    
}

}

