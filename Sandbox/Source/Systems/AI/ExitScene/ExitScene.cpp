#include "ExitScene.h"
#include "../../UnitManager.h"
#include "../AITools.h"
namespace DeltaEngine
{
    void ExitScene::Initialize()
    {
        ExitPoint = Vector2{ 41.0f,-1.063f };
    }

    void ExitScene::Update()
    {
        if (em.IsEntityValid(UnitManager::GetPlayerID()) && em.HasComponent<Player>(UnitManager::GetPlayerID()))
        {
            EntityID p = UnitManager::GetPlayerID();
            auto& transform = em.GetComponent<Transform>(p);
            if (AITools::EntityisAtPointInX(p, ExitPoint.x, 1.0f))
            {
                em.RemoveComponent<Input>(p);
                em.GetComponent<RigidBody>(p).Direction = Vector2::right();
            }
        }

    }

}
