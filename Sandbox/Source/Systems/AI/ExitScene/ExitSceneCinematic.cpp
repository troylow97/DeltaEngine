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
namespace DeltaEngine
{
    void ExitSceneCinematic::Initialize()
    {
        ExitPoint = Vector2{ 41.0f,-1.063f };
        ExitPoint = Vector2{ 44.0f,-1.063f };
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
            }

            if (ExitPointTriggered && !StopPointTriggered)
            {
              static float timer = 0;
              timer += GetEnv().pClock->DeltaTime();
              Camera::allCameras[0]->fadeColorAmt = timer;
            }
        }

    }

    void ExitSceneCinematic::LateUpdate()
    {
	    
    }
}
