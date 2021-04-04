/**********************************************************************************
* \file   HealthPickupSystem.cpp
* \brief  The file contains the system for healthpickups
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "HealthPickupSystem.h"
#include "../AI/AITools.h"
#include "Core/GlobalStruct.h"
#include "../UnitManager.h"

namespace DeltaEngine
{
	float HealthPickupSystem::PickupChance = 100.0f;
	
	void HealthPickupSystem::Initialize()
	{
		PickupChance = 100.0f;
	}

	void HealthPickupSystem::Update()
	{

	}

	void HealthPickupSystem::LateUpdate()
	{
		
	}
	
	void HealthPickupSystem::SpawnHealthOrbOnDeath(const Vector2 position)
	{
		
		if(Random::RandomFloatRange(0.0f,100.0f) < PickupChance)
		{
			std::cout << "SPAWNING" << std::endl;
			EntityID orb = SpawnHealthOrb(position);
			PickupChance = 100.0f;
		}
		else
		{
			PickupChance += 5.0f;
		}
	}

	EntityID HealthPickupSystem::SpawnHealthOrb(const Vector2 position)
	{
		auto& em = env.pECS->GetWorld().GetEntityManager();
        const EntityID pickup = em.CreateEntity<Collider, Lifespan, RigidBody,Renderer2D,Image,Animator,State>();
        em.GetComponent<Transform>(pickup).position = position;
        em.GetComponent<RigidBody>(pickup).Mass = 5.0f;
        em.GetComponent<Transform>(pickup).scale = {1.0f,1.0f,0.0f};
        em.GetComponent<Lifespan>(pickup).Timer = {5.0f};
        em.GetComponent<RigidBody>(pickup).hasGravity = false;
		em.GetComponent<Collider>(pickup).isTrigger = true;
        em.GetComponent<EntityType>(pickup).type = EntityCategory::E_HEALTHUP;
        em.GetComponent<RigidBody>(pickup).FrictionCoeff = 0.0f;
		em.GetComponent<Renderer2D>(pickup).m_Wireframe = false;
		em.GetComponent<Renderer2D>(pickup).m_SortingLayer = 2;
		em.GetComponent<Animator>(pickup).m_ControllerKey = "Animation/HealthPickup";
		em.GetComponent<Image>(pickup).m_Sprite.m_Key = "Textures/HEALTH_PICKUP";
        return pickup;
	}

}
