//#pragma once
//#include "Core/Math/Vector.h"
//#include "CollisionSystem.h"
//#include "Collider.h"
//
//namespace DeltaEngine
//{
//    DEFINE_SYSTEM(PhysicsSystem, RigidBody, Transform, Collider)
//    public:
//        //PhysicsSystem() = default;
//        //~PhysicsSystem() = default;
//        //CollisionSystem collision_system;
//
//        virtual void update() override;
//        virtual void late_update() override;
//        void Init();
//        void UpdateComponents();
//        void UpdateVelocity(Entity id)
//        {
//        	//ecs->get_component<Transform>(id).position = ecs->get_component<RigidBody>(id).Velocity;
//        }
//        void Gravity(Entity id)
//        {
//        	Vector2 Gravity = { 0,-2 };
//        	//ecs->get_component<RigidBody>(id).Velocity - Gravity;
//        }
//
//    END_DEFINE_SYSTEM(PhysicsSystem)
//}
//
//
////namespace DeltaEngine
////{
////	class DE_API PhysicsSystem
////	{
////	public:
////		CollisionSystem collision_system;
////
////		void OnUpdate(GameClock dt)
////		{
////			em.Query;
////			for (em.)
////			{
////				Gravity(ref);
////			}
////			collision_system.Update();
////			for (DeltaEngine::Entity ref : entities)
////			{
////				UpdateVelocity(ref);
////			}
////
////		}
////		void Init();
////		void UpdateVelocity(Entity id)
////		{
////			//ecs->get_component<Transform>(id).position = ecs->get_component<RigidBody>(id).Velocity;
////		}
////		void Gravity(Entity id)
////		{
////			Vector2 Gravity = { 0,-2 };
////			//ecs->get_component<RigidBody>(id).Velocity - Gravity;
////		}
////	};
////}
