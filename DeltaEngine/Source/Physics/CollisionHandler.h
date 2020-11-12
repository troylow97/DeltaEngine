#pragma once
#include "Core/TypeAlias.h"
#include "Core/Containers/ActionList.h"
#include "ECS/Entities.h"

namespace DeltaEngine
{

	class CollisionHandler
	{

		Util::ActionList<EntityID&, EntityID&> m_enter;
		Util::ActionList<EntityID&, EntityID&> m_stay;
		Util::ActionList<EntityID&, EntityID&> m_exit;

		void OnEnter(EntityID&, EntityID&);
		void OnStay(EntityID&, EntityID&);
		void OnExit(EntityID&, EntityID&);
		friend struct CollisionSystem;
	public:
		CollisionHandler() = default;


		size_t RegisterOnEnter(Action<EntityID&, EntityID&> action);
		size_t RegisterOnStay(Action<EntityID&, EntityID&> action);
		size_t RegisterOnExit(Action<EntityID&, EntityID&> action);

		void UnregisterOnEnter(size_t id);
		void UnregisterOnStay(size_t id);
		void UnregisterOnExit(size_t id);
	};
}
