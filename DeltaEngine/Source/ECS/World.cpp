#include "World.h"

#include "../../../Sandbox/Source/Systems/AI/AI_StateMachine.h"
#include "Physics/PhysicsSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/PhysicsDrawSystem.h"
#include "Systems/RenderSystem.h"

namespace DeltaEngine
{
  bool World::SystemExist(size_t digest)
  {
    if (systems.find(digest) == systems.end())
      return false;
    return true;
  }

  World::World(): em(std::make_unique<EntityManager>())
  {
    CreateSystems<InputSystem, AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem,
                  PhysicsDrawSystem>();
#ifndef DE_EDITOR
  m_pause = false;
#endif
  }

  void World::SetPause(bool pause)
  {
    m_pause = pause;
  }


  EntityManager& World::GetEntityManager() const
  {
    return *em;
  }

  void World::InitSystems()
  {
    for (auto& [hash, system] : systems)
      system->Initialize();
  }

  void World::ShutdownSystems()
  {
    for (auto& [hash, system] : systems)
      system->Shutdown();
  }

  void World::Update()
  {
     //Input System Update
    systems[CHash::Hash<InputSystem>().digest]->Update();
    systems[CHash::Hash<AISystem>().digest]->Update();

    if (!m_pause)
    {
      // Logic Update
      for (auto hash : update_sequence)
        systems[hash]->Update();

      // Physics Update
      systems[CHash::Hash<PhysicsSystem>().digest]->Update();
      systems[CHash::Hash<CollisionSystem>().digest]->Update();

      // Logic Update
      for (auto hash : update_sequence)
        systems[hash]->LateUpdate();
    }
    systems[CHash::Hash<AnimationSystem>().digest]->Update();
    systems[CHash::Hash<RenderSystem>().digest]->Update();
    systems[CHash::Hash<PhysicsDrawSystem>().digest]->Update();
    systems[CHash::Hash<RenderSystem>().digest]->LateUpdate();
  }

  void World::LateUpdate()
  {
    for (auto hash : late_update_sequence)
      systems[hash]->LateUpdate();
  }

  void World::Save( std::string filename )
  {
    JsonFile file;
    file.StartWriter( filename ).WriteEntities( *em ).EndWriter();
  }

  void World::Load(std::string filename)
  {
    JsonFile file;
    file.StartReader(filename).LoadEntities(*em).EndReader();
  }
}
