#pragma once
#include "Core/Math/Vector.h"
#include "RigidBody.h"
#include "Core/Math/Transform.h"
#include "CollisionSystem.h"
#include "../Application.h"
namespace DeltaEngine
{
	class DE_API PhysicsSystem : public System
	{
	public:
		CollisionSystem collision_system;

		void OnUpdate(GameClock dt)
		{
			for (DeltaEngine::Entity ref : entities)
			{
				Gravity(ref);
			}
			collision_system.Update();
			for (DeltaEngine::Entity ref : entities)
			{
				UpdateVelocity(ref);
			}

		}
		void Init();
		void UpdateVelocity(Entity id)
		{
			//ecs->get_component<Transform>(id).position = ecs->get_component<RigidBody>(id).Velocity;
		}
		void Gravity(Entity id)
		{
			Vector2 Gravity = { 0,-2 };
			//ecs->get_component<RigidBody>(id).Velocity - Gravity;
		}
	};
}
