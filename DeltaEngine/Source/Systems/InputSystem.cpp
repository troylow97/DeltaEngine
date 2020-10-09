#include "InputSystem.h"

namespace DeltaEngine
{
	void InputSystem::update()
	{
        if (InputManager::get()->isKeyPressed(DEVK_A))
        {
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1, Input& i1)
                {
                    i1.previousKey = DEVK_A;
                    r1.Velocity = { -1, 0 };
                });
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, Animator& a)
                {
                    a.SetFloat("Speed", 1.0f);
                });
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, SpriteRenderer& s)
                {
                    s.m_FlipX = true;
                });
        }
        else if (InputManager::get()->isKeyReleased(DEVK_A))
        {
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, Animator& a)
                {
                    a.SetFloat("Speed", 0.0f);
                });
        }
        if (InputManager::get()->isKeyPressed(DEVK_D))
        {
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1, Input& i1)
                {
                    i1.previousKey = DEVK_D;
                    r1.Velocity = { 1, 0 };
                });
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, Animator& a)
                {
                    a.SetFloat("Speed", 1.0f);
                });
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, SpriteRenderer& s)
                {
                    s.m_FlipX = false;
                });
        }
        else if (InputManager::get()->isKeyReleased(DEVK_D))
        {
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, Animator& a)
                {
                    a.SetFloat("Speed", 0.0f);
                });
        }
        if (InputManager::get()->isKeyPressed(DEVK_W))
        {
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1, Input& i1)
                {
                    i1.previousKey = DEVK_W;
                    r1.Velocity = { 0, 1 };
                });
        }
        if (InputManager::get()->isKeyPressed(DEVK_S))
        {
            env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1, Input& i1)
                {
                    i1.previousKey = DEVK_S;
                    r1.Velocity = { 0, -1 };
                });
        }
        if (InputManager::get()->isKeyTriggered(DEVK_BACKSLASH)) // '\'
        {
            if (InputManager::get()->getShowLine() == false)
            {
                //std::cout << "V is triggered and line is shown" << std::endl;
                InputManager::get()->setShowLine(true);
            }
            else if (InputManager::get()->getShowLine() == true)
            {
                //std::cout << "V is triggered and line is NOT shown" << std::endl;
                InputManager::get()->setShowLine(false);
            }
        }

        if (InputManager::get()->isKeyTriggered(DEVK_P))
        {
            em.for_each([&](EntityID& id, Input& input)
                {
                    auto t2 = em.get_component<Transform>(id);
                    auto circleEntity = env.pECS->world().get_entity_manager().create_entity<Transform, RigidBody, Collider>();
                    auto& circleT = env.pECS->world().get_entity_manager().get_component<Transform>(circleEntity);
                    auto& circleR = env.pECS->world().get_entity_manager().get_component<RigidBody>(circleEntity);
                    auto& circleC = env.pECS->world().get_entity_manager().get_component<Collider>(circleEntity);

                    circleT.position = Vector3(t2.position.x, t2.position.y - 0.25f);
                    circleT.scale = Vector3(0.1, 0.1);
                    circleR.hasGravity = true;
                    circleC.type = ColliderType::CIRCLE;
                });
        }

        // gets the coordinates of the mouse, good for debugging
        if (InputManager::get()->onMouseMove())
        {
            InputManager::get()->currentPosition();
        }
	}
}