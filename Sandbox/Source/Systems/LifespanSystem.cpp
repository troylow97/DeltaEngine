/**********************************************************************************
* \file   LifespanSystem.cpp
* \brief  The file contains the system for updating entities health
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "LifespanSystem.h"

#include "UnitManager.h"
#include "Audio/AudioEngine.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/GameClock/EngineClock.h"

namespace DeltaEngine
{

	void LifespanSystem::Update()
	{
		em.ForEach([&](EntityID& id, EntityType& et, Lifespan& l)
			{
				if (l.Timer < 0.0f)
					DestroyedEntities.push_back(id);
				else
					l.Timer -= env.pClock->FixedDeltaTime();
			});

		for(auto& ref : DestroyedEntities)
			em.DestroyEntity(ref);

		DestroyedEntities.clear();
		
		Profiler::Instance().Record("Lifespan System");
	}

	void LifespanSystem::LateUpdate()
	{}
}
