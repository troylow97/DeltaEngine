#pragma once
#include "CollisionHandler.h"
#include "ECS/ECSModule.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Core/Math/Transform.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(CollisionSystem, Collider, RigidBody, Transform)

    using EntityPair = std::pair<EntityID, EntityID>;


    std::vector<EntityPair> CurrentPair;
    std::vector<EntityPair> PreviousPair;

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