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
        if ( c2.isCollideable )
        {
          if ( id1.index != id2.index )
          {
            c2.center = t2.position;
            c2.size = t2.scale;
            Manifold m { c1,c2 };
            if ( CollisionIntersection_Main( c1, r1, c2, r2, m ) )
            {
              CollisionPairInfo temp { m,id1,id2 };
              //DeltaEngine_CORE_TRACE("COLLISION");
              current_manifold_vector.push_back( temp );
              //if(c1.type == ColliderType::CIRCLE && c2.type == ColliderType::CIRCLE)
              //	std::cout << "Collided" << std::endl;
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
          collision_handler.OnStay( it1->m.A );
          collision_handler.OnStay( it1->m.B );
          Handled = true;
          //DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON STAY");
        }
      }

      if ( !Handled )
      {
        collision_handler.OnEnter( it1->m.A );
        collision_handler.OnEnter( it1->m.B );
        //DeltaEngine_CORE_TRACE("COLLISION_HANDLING: ON ENTER");
      }
    }
  }



  if ( !Handled && !old_manifold_vector.empty() )
  {
    //handle previous pair exit
    for ( auto it3 = old_manifold_vector.begin(); it3 != old_manifold_vector.end(); it3++ )
    {
      collision_handler.OnExit( it3->m.A );
      collision_handler.OnExit( it3->m.B );
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
          CollisionResponse_Main( c1, r1, t1, c2, r2, t2, it1->m );
          //AABB_CollisionResponse(c1, r1, c2, r2);

          //float r1Magnitude = (r1.Velocity.x * r1.Velocity.x + r1.Velocity.y * r1.Velocity.y);
          //float r2Magnitude = (r2.Velocity.x * r2.Velocity.x + r2.Velocity.y * r2.Velocity.y);
          //
          //Vector2 ResultVec;
          //ResultVec.x = r1.Velocity.x + r2.Velocity.x;
          //ResultVec.y = r1.Velocity.y + r2.Velocity.y;
          //
          //std::cout << "normal is:: " << it1->m.normal.x << "," << it1->m.normal.y << std::endl;
          //std::cout << "pen is:: " << it1->m.penetration << std::endl;
          //
          //r1.Velocity = ResultVec * (r2.Mass / (r1.Mass + r2.Mass));
          //r2.Velocity = ResultVec * (r1.Mass / (r1.Mass + r2.Mass));



          //Calculate relative velocity
           //Vector2 rv = r2.Velocity - r1.Velocity;
           //Vector2 normal = it1->m.normal;
           //
           //// Calculate relative velocity in terms of the normal direction
           //float velAlongNormal = Vector2DotProduct(rv, normal);
           //
           //// Do not resolve if velocities are separating
           //if (velAlongNormal > 0)
           //	return;
           //
           //// Calculate restitution
           //float e = Math::math_min(r1.Restituition, r2.Restituition);
           //
           //// Calculate impulse scalar
           //float j = -(1 + e) * velAlongNormal;
           //j /= 1 / r1.Mass + 1 / r2.Mass;
           //
           //// Apply impulse
           //Vector2 impulse = normal * j;
           //r1.Velocity -= (impulse * 1 / r1.Mass);
           //r2.Velocity += (impulse * 1 / r2.Mass);
        }

      } );
    } );
  }

}

}

