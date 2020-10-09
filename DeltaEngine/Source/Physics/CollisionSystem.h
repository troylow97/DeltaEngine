#pragma once
#include "CollisionHandler.h"
#include "RigidBody.h"
#include "ECS/Components/Transform.h"
#include "Manifold.h"
#include "ECS/Components/RigidBody.h"

namespace DeltaEngine
{
struct CollisionPairInfo
{
  Manifold m;
  EntityID id1;
  EntityID id2;

  CollisionPairInfo(Manifold man,EntityID i1,EntityID i2) :
    m{man},
    id1{i1},
    id2{i2}
  {}
};

DEFINE_SYSTEM(CollisionSystem, Collider, RigidBody, Transform)

    using CollisionPair = std::pair<EntityID, EntityID>;
    std::vector<CollisionPair> CurrentPair;
    std::vector<CollisionPair> PreviousPair;
    void CollisionIntersectionCheck();
    void CollisionHandling();
    void CollisionResolution();

    public:
    virtual void update() override;
    virtual void late_update() override;
    //CollisionSystem() = default;
    //~CollisionSystem() = default;
    CollisionHandler collision_handler;
    void Init();


  END_DEFINE_SYSTEM(CollisionSystem)
}

/*
Collision System
Members: Vector of collisionPair with current or previous.
Pre Current
1 0: Exit
1 1: Stay
0 1: Enter
 - Update loop through all the IDs with Colliders to check for collision once there is create a collision pair and pass into the vector of current collisionPair
 - UpdateCheck -> Check with previous one.

 - UpdateLate -> CollisionHandling

 - CollisionPair -> Take in two Entity ID


*/