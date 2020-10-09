#pragma once
#include "CollisionHandler.h"
#include "ECS/ECSModule.h"
#include "RigidBody.h"
#include "Core/Math/Transform.h"
#include "Manifold.h"

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



    std::vector<CollisionPairInfo> CurrentManifoldVector;
    std::vector<CollisionPairInfo> OldManifoldVector;

    void CollisionSystem::CollisionIntersectionCheck();
    void CollisionSystem::CollisionHandling();
    void CollisionSystem::CollisionResolution();

    public:
    virtual void CollisionSystem::update() override;
    virtual void CollisionSystem::late_update() override;
    CollisionSystem::CollisionSystem() = default;
    CollisionSystem::~CollisionSystem() = default;
    CollisionHandler collision_handler;
    void CollisionSystem::Init();


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