#include <DeltaEngine.h>
#include "EntryPoint.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>
#include "AI/AI_StateMachine.h"
#include "Physics/CollisionSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/PhysicsDrawSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/AttackSystem.h"
#include "Systems/LifespanSystem.h"
#include "CollisionHandlingFunctions.h"

class Sandbox : public DeltaEngine::Application
{
public:
	Sandbox()
	{
		CollisionSystem::collision_handler.RegisterOnStay(TakeDamage);

		EntityID first = env.pECS->GetWorld().GetEntityManager().CreateEntity();
		env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(first);
		env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(first);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(first);
		env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(first);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Image>(first);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Input>(first);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Attack>(first);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Health>(first);

		env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(first).type = entity_type::E_PLAYER;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(first).MaxHealth = 5000;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(first).CurrentHealth = 5000;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(first).size = { 0.5,0.5 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(first).type = ColliderType::BOX;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(first).position = { 0.5,2.0,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(first).scale = { 0.5,0.5,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).Movespeed = 30.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).Mass = 15.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).isMoveable = true;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).FrictionCoeff = 1.5f;

		EntityID sec = env.pECS->GetWorld().GetEntityManager().CreateEntity();
		//env.pECS->GetWorld().GetEntityManager().AddComponent<AI>(sec);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(sec);
		env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(sec);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(sec);
		env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(sec);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Health>(sec);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Attack>(sec);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Image>(sec);
		env.pECS->GetWorld().GetEntityManager().AddComponent<AI>(sec);
		env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(sec).key = "idle_monster";
		env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(sec).transition = "null";
		env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(sec).type = entity_type::E_ENEMY;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(sec).size = { 0.5,0.5 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(sec).type = ColliderType::BOX;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sec).position = { -2,1,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sec).scale = { 0.5,0.5,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).Movespeed = 20.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).Mass = 30.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).isMoveable = true;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).Restitution = 0.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).FrictionCoeff = 1.5f;

		EntityID third = env.pECS->GetWorld().GetEntityManager().CreateEntity();
		env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(third);
		env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(third);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(third);
		env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(third);

		env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(third).type = ColliderType::BOX;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(third).position = { 0,-3,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(third).scale = { 30.5,0.5,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(third).Movespeed = 50.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(third).Mass = 1500.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(third).isMoveable = false;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(third).Restitution = 0.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(third).type = entity_type::E_WALL;

		EntityID fourth = env.pECS->GetWorld().GetEntityManager().CreateEntity();
		env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(fourth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(fourth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(fourth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(fourth);
		env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(fourth).type = ColliderType::BOX;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(fourth).position = { -5,0,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(fourth).scale = { 0.5,10.5,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fourth).Movespeed = 50.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fourth).Mass = 1500.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fourth).isMoveable = false;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fourth).Restitution = 0.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(fourth).type = entity_type::E_WALL;


		EntityID fifth = env.pECS->GetWorld().GetEntityManager().CreateEntity();
		env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(fifth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(fifth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(fifth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Health>(fifth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(fifth);
		env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(fifth).type = entity_type::E_ENEMY;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(fifth).type = ColliderType::BOX;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(fifth).position = { -2,2,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(fifth).scale = { 0.5,0.5,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fifth).Movespeed = 50.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fifth).Mass = 50.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fifth).isMoveable = true;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fifth).Restitution = 0.0f;

		EntityID sixth = env.pECS->GetWorld().GetEntityManager().CreateEntity();
		env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(sixth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(sixth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(sixth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<Health>(sixth);
		env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(sixth);
		env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(sixth).type = entity_type::E_ENEMY;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(sixth).type = ColliderType::BOX;
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sixth).position = { -2,3,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sixth).scale = { 0.5,0.5,0 };
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sixth).Movespeed = 50.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sixth).Mass = 50.0f;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sixth).isMoveable = true;
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sixth).Restitution = 0.0f;

		env.pECS->GetWorld().Save("Entities2.json");

		env.pECS->GetWorld().CreateSystems<InputSystem, AISystem, AttackSystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem,LifespanSystem>();
		env.pECS->GetWorld().SetUpdateSequence<InputSystem, AISystem, AttackSystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem, LifespanSystem>();
		env.pECS->GetWorld().SetLateUpdateSequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();




		env.pECS->GetWorld().InitSystems();
	}

	~Sandbox()
	{

	}
};

DeltaEngine::Application* DeltaEngine::CreateApplication()
{
	return new Sandbox();
}