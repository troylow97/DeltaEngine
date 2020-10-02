#pragma once
#include "CollisionHandler.h"
#include "OECS/Types.h"
#include "DE_API.h"
#include "OECS/System.h"

namespace DeltaEngine
{

	class CollisionSystem : public System
	{
        using CollisionPair = std::pair<Entity, Entity>;
        std::vector<CollisionPair> CurrentPair;
        std::vector<CollisionPair> PreviousPair;

    public:
        CollisionHandler collision_handler;
        
        void CollisionIntersectionCheck();
        void CollisionHandling();
        void CollisionResolution();
        void Init();
        void Update();
	};
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