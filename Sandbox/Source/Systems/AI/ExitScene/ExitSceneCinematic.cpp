/**********************************************************************************
* \file   ExitSceneCinematic.cpp
* \brief  The file contains the code for exiting the scene
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ExitSceneCinematic.h"
#include "../../UnitManager.h"
#include "../AITools.h"
#include "DeltaEngine.h"
#include "../../EnemySpawner/EnemySpawner.h"
#include "Systems/OCullSystem.h"
namespace DeltaEngine
{
    void ExitSceneCinematic::Initialize()
    {
        ExitPoint = Vector2{ 41.0f,-1.063f };
        StopPoint = Vector2{ 47.0f,-1.063f };
        timer = 0.0f;
        ExitPointTriggered = false;
        StopPointTriggered = false;
    }

    void ExitSceneCinematic::Update()
    {
        if (em.IsEntityValid(UnitManager::GetPlayerID()) && em.HasComponent<Player>(UnitManager::GetPlayerID()))
        {
            EntityID p = UnitManager::GetPlayerID();
            auto& transform = em.GetComponent<Transform>(p);
            if (!ExitPointTriggered && AITools::EntityisAtPointInX(p, ExitPoint.x, 1.0f))
            {
                em.RemoveComponent<Input>(p);
                em.GetComponent<RigidBody>(p).Direction = Vector2::right();
                ExitPointTriggered = true;
            }
            else if(!StopPointTriggered && AITools::EntityisAtPointInX(p, StopPoint.x, 1.0f))
            {
                em.GetComponent<RigidBody>(p).Direction = Vector2{ 0,0 };
                StopPointTriggered = true;
            }
            if (StopPointTriggered)
            {
                timer += GetEnv().pClock->FixedDeltaTime();
                Camera::allCameras[0]->fadeColorAmt = timer;
            }
            if (timer >= 5.0f)
                CreditsStart();
        }
    }

    void ExitSceneCinematic::CreditsStart()
    {
        timer = 0.0f;
        ExitPointTriggered = false;
        StopPointTriggered = false;
        env.pECS->GetWorld().FindOrCreateSystem<EnemySpawner>().Initialize();
        env.pECS->GetWorld().GetEntityManager().Clear();
        OCullSystem::Enable(false);
        env.pECS->GetWorld().Load("World/CreditsRolling.json");
    }

    void ExitSceneCinematic::LateUpdate()
    {
	    
    }
}
