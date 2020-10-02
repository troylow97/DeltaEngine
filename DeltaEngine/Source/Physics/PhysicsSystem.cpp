#include "PhysicsSystem.h"
#include "RigidBody.h"
#include "Core/Math/Transform.h"
#include "Collider.h"

namespace DeltaEngine
{
	void PhysicsSystem::Init()
	{
		PhysicsSystem* physics_system = ecs->register_system<PhysicsSystem>();
		{
			DeltaEngine::BitSignature signature;
			signature.set(ecs->get_component_type<RigidBody>());
			signature.set(ecs->get_component_type<Transform>());
			signature.set(ecs->get_component_type<Collider*>());
			ecs->set_system_signature<PhysicsSystem>(signature);
		}
	}

}