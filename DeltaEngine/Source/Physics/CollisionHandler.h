#pragma once
#include "Core/TypeAlias.h"
#include "Core/Containers/ActionList.h"
#include "ECS/Entities.h"

namespace DeltaEngine
{

	class CollisionHandler
	{

		Util::ActionList<EntityID&> m_enter;
		Util::ActionList<EntityID&> m_stay;
		Util::ActionList<EntityID&> m_exit;

		void OnEnter(EntityID& id);
		void OnStay(EntityID& id);
		void OnExit(EntityID& id);
		friend struct CollisionSystem;
	public:
		CollisionHandler() = default;


		size_t RegisterOnEnter(Action<EntityID&> action);
		size_t RegisterOnStay(Action<EntityID&> action);
		size_t RegisterOnExit(Action<EntityID&> action);

		void UnregisterOnEnter(size_t id);
		void UnregisterOnStay(size_t id);
		void UnregisterOnExit(size_t id);
	};
}
