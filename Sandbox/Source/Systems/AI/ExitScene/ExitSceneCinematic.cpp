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
#include "../../UISystem.h"
#include "Systems/OCullSystem.h"
namespace DeltaEngine
{
    void ExitSceneCinematic::Initialize()
    {
        ExitPoint = Vector2{ 41.0f,-1.063f };
        StopPoint = Vector2{ 47.0f,-1.063f };
        timer = 0.0f;
        credits_timer = 0.0f;
        ExitPointTriggered = false;
        StopPointTriggered = false;
        CreditsRolledTriggered = false;
        MainMenuTriggered = false;
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

        if (CreditsRolledTriggered)
        {
            em.ForEach([&](EntityID& id, EntityName& en, Transform& t)
            {
                if (en.name == "CreditsToBeRolled")
                {
                    auto& pos = em.GetComponent<Transform>(id);

                    if (pos.position.y <= 7.8f)
                    {
                        pos.position.y += (GetEnv().pClock->FixedDeltaTime() * 0.1f);
                        std::cout << " pos.position.y is " << pos.position.y << std::endl;
                    }
                    else
                    {
                        CreditsRolledTriggered = false;
                        MainMenuTriggered = true;
                    }
                }

            });
        }
        if (MainMenuTriggered)
        {
            LoadMainMenuAgain();
            MainMenuTriggered = false;
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
        CreditsRolledTriggered = true;
    }

    void ExitSceneCinematic::LoadMainMenuAgain()
    {
        env.pECS->GetWorld().FindOrCreateSystem<UISystem>().Initialize();
        env.pECS->GetWorld().GetEntityManager().Clear();
        env.pECS->GetWorld().Load("World/MainMenu.json");
    }

    void ExitSceneCinematic::LateUpdate()
    {
	    
    }
}
