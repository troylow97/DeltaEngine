/**********************************************************************************
* \file   CollisionSystem.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "CollisionHandler.h"
#include "ECS/ECSModule.h"
#include "Components/RigidBody.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Manifold.h"

namespace DeltaEngine
{
  struct CollisionPairInfo
  {
    Manifold m;
    EntityID id1;
    EntityID id2;

    CollisionPairInfo(Manifold man, EntityID i1, EntityID i2) :
      m{man},
      id1{i1},
      id2{i2}
    {}
  };

  DEFINE_SYSTEM(CollisionSystem, Collider, RigidBody, Transform)

    std::vector<CollisionPairInfo> current_manifold_vector;
    std::vector<CollisionPairInfo> old_manifold_vector;

    void CollisionSystem::CollisionIntersectionCheck();
    void CollisionSystem::CollisionHandling();
    void CollisionSystem::CollisionResolution();

    //void CollisionSystem::CollisionResponseMain(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m);
    //void CollisionSystem::CollisionResponse_Sub_Box(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m);
    //void CollisionSystem::CollisionResponse_Sub_Circle(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m);

    void CollisionSystem::CollisionResponse(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m);
    //void CollisionSystem::CollisionResponse_BoxCircle(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m);
    //void CollisionSystem::CollisionResponse_CircleCircle(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m);

  public:
    
    void CollisionSystem::Update() override;
    void CollisionSystem::LateUpdate() override;
    void CollisionSystem::Initialize() override;
    CollisionSystem::CollisionSystem() = default;
    CollisionSystem::~CollisionSystem() = default;

    static CollisionHandler collision_handler;


  END_DEFINE_SYSTEM(CollisionSystem)
}

/*
Collision System
Members: Vector of EntityPair with current or previous.
Pre Current
1 0: Exit
1 1: Stay
0 1: Enter
- Update loop through all the IDs with Colliders to check for collision once there is create a collision pair and pass into the vector of current EntityPair
- UpdateCheck -> Check with previous one.
- UpdateLate -> CollisionHandling
- EntityPair -> Take in two Entity ID
*/
