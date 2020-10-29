#include "CollisionSystem.h"
#include "CollisionResponseCallbacks.h"
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
  CollisionIntersectionCheck();
  CollisionHandling();
  CollisionResolution();

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
      c1.center = t1.position;
      c1.size = t1.scale;
      em.ForEach( [&]( EntityID id2, RigidBody &r2, Transform &t2, Collider &c2 )
      {
        if (c2.isCollideable)
        {
          if ( id1.index != id2.index )
          {
            c2.center = t2.position;
            c2.size = t2.scale;
            Manifold m;
            if (CollisionIntersection_Main(c1, r1, c2, r2, m))
            {
                current_manifold_vector.push_back({ c1,c2, m,id1,id2});
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
          collision_handler.OnStay( it1->a );
          collision_handler.OnStay( it1->b );
          Handled = true;
          //DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON STAY");
        }
      }

      if ( !Handled )
      {
        collision_handler.OnEnter( it1->a );
        collision_handler.OnEnter( it1->b );
        //DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON ENTER");
      }
    }
  }



  if ( !Handled && !old_manifold_vector.empty() )
  {
    //handle previous pair exit
    for ( auto it3 = old_manifold_vector.begin(); it3 != old_manifold_vector.end(); it3++ )
    {
      collision_handler.OnExit( it3->a );
      collision_handler.OnExit( it3->b );
      //DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON EXIT");
    }
  }

}

void CollisionSystem::CollisionResolution()
{


    for ( auto it1 = current_manifold_vector.begin(); it1 != current_manifold_vector.end(); it1++ )
    {
      em.ForEach( [&]( EntityID id1, RigidBody &r1, Transform &t1, Collider &c1 )
      {
        em.ForEach( [&]( EntityID id2, RigidBody &r2, Transform &t2, Collider &c2 )
        {
          if ( it1->id1.index == id1.index && it1->id2.index == id2.index )
          {
              CollisionResponse(c1,r1,c2, r2, it1->m);
          }
    
        } );
      } );
    }
//        //Resolving penetrations is a tricky problem with large stacks. The only real way to get 
//        //good results is by considering all contacts at once at solving them together. This method basically 
//        //just keeps poking the bodies until the penetrations are below the penetration slop epsilon. 
//        //Better solutions involve building contact graphs, shock propagation and constraint based
//        //solvers.
//    unsigned iterationsRun = 0;
//    unsigned maxIterations = current_manifold_vector.size() * 5;
//    const float positionEpsilon = 0.8f;
//
//    while (iterationsRun < maxIterations)
//    {
//        // Find biggest penetration greater than
//        //the correction epsilon
//        float maxPenetration = positionEpsilon;
//        unsigned contactIndex = current_manifold_vector.size();
//        for (unsigned int i = 0; i < current_manifold_vector.size(); i++)
//        {
//            if (current_manifold_vector[i].m.penetration > maxPenetration)
//            {
//                maxPenetration = current_manifold_vector[i].m.penetration;
//                contactIndex = i;
//            }
//        }
//        if (contactIndex == current_manifold_vector.size()) break;
//
//         for (unsigned int i = 0; i < current_manifold_vector.size(); i++)
//         {
//             if (current_manifold_vector[i].m.penetration == maxPenetration )
//             {
//                 em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
//                     {
//                         em.ForEach([&](EntityID id2, RigidBody& r2, Transform& t2, Collider& c2)
//                             {
//                                 if (id1.index == current_manifold_vector[i].id1.index && id2.index == current_manifold_vector[i].id2.index)
//                                 {
//                                     ResolvePenetration(r1, t1, r2, t2, current_manifold_vector[i].m);
//                                     contactIndex = i;
//                                 }
//
//                             });
//                     });
//                 break;
//             }
//         }
//
//        // Update the penetrations for all related contacts
//        Vector2* movement = current_manifold_vector[contactIndex].m.Movement;
//        for (unsigned int i = 0; i < current_manifold_vector.size(); i++)
//        {
//            if (current_manifold_vector[i].id1.index == current_manifold_vector[contactIndex].id1.index)
//                current_manifold_vector[i].m.penetration -= Vector2DotProduct(movement[0], current_manifold_vector[i].m.normal);
//            else if (current_manifold_vector[i].id1.index == current_manifold_vector[contactIndex].id2.index)
//                current_manifold_vector[i].m.penetration -= Vector2DotProduct(movement[1], current_manifold_vector[i].m.normal);
//
//
//            if (current_manifold_vector[i].id2.index == current_manifold_vector[contactIndex].id1.index)
//                current_manifold_vector[i].m.penetration += Vector2DotProduct(movement[0], current_manifold_vector[i].m.normal);
//            else if (current_manifold_vector[i].id2.index == current_manifold_vector[contactIndex].id2.index)
//                current_manifold_vector[i].m.penetration += Vector2DotProduct(movement[1], current_manifold_vector[i].m.normal);
//
//        }
//        ++iterationsRun;
//    }
//   iterationsRun = 0;
//   while (iterationsRun < maxIterations)
//   {
//       // Find the contact with the largest closing velocity;
//       float maxVelocity = FLT_MAX;
//       unsigned contactIndex = current_manifold_vector.size();
//           for (unsigned int i = 0; i < current_manifold_vector.size(); i++)
//           {
//               em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
//                   {
//                       em.ForEach([&](EntityID id2, RigidBody& r2, Transform& t2, Collider& c2)
//                           {
//                               float sepVel = Vector2DotProduct(r1.Velocity - r2.Velocity, current_manifold_vector[i].m.normal);
//                               if (sepVel < 0 && sepVel < maxVelocity)
//                               {
//                                   maxVelocity = sepVel;
//                                   contactIndex = i;
//                               }
//   
//                           });
//                   });
//           }
//
//           // Do we have anything worth resolving?
//           if (contactIndex == current_manifold_vector.size()) break;
//
//       // Resolve this contact velocity
//       for (unsigned int i = 0; i < current_manifold_vector.size(); i++)
//       {
//           if (contactIndex == i)
//           {
//               em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
//                   {
//                       em.ForEach([&](EntityID id2, RigidBody& r2, Transform& t2, Collider& c2)
//                           {
//                               ResolveContactVelocity(r1, r2, current_manifold_vector[i].m);
//                           });
//                   });
//           }
//   
//       }
//   
//   
//       ++iterationsRun;
//   }
}

}

