/**********************************************************************************
* \file   World.cpp
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "World.h"

#include "../../../Sandbox/Source/Systems/AI/AI_StateMachine.h"
#include "Physics/PhysicsSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/OCullSystem.h"
#include "Systems/PhysicsDrawSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/ParticleSystem.h"

namespace DeltaEngine
{
bool World::SystemExist( size_t digest )
{
  if ( systems.find( digest ) == systems.end() )
    return false;
  return true;
}

World::World() : em( std::make_unique<EntityManager>() )
{
  DeltaEngine_CORE_INFO( "Initializing World..." );
  CreateSystems<InputSystem, AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem,
    PhysicsDrawSystem, ParticleSystem, OCullSystem>();
  DeltaEngine_CORE_INFO( "Initializing World successful" );

#ifndef DE_EDITOR
  m_pause = false;
#endif
}

  void World::SetPause(bool pause)
  {
    m_pause = pause;
  }


EntityManager &World::GetEntityManager() const
{
  return *em;
}

void World::InitSystems()
{
  for ( auto &[hash, system] : systems )
    system->Initialize();
}

void World::ShutdownSystems()
{
  DeltaEngine_CORE_INFO( "Shutting down World's Systems" );
  for ( auto &[hash, system] : systems )
    system->Shutdown();
}

void World::Run()
{
  //Input System Update
  systems[CHash::Hash<InputSystem>().digest]->Update();

  if ( !m_pause )
  {
    // State Machine Update
    systems[CHash::Hash<AISystem>().digest]->Update();


    // Physics Update
    systems[CHash::Hash<PhysicsSystem>().digest]->Update();
    systems[CHash::Hash<CollisionSystem>().digest]->Update();

    // Logic Update
    Update();

    systems[CHash::Hash<AnimationSystem>().digest]->Update();

    // Logic Late Update
    LateUpdate();
  }
  systems[CHash::Hash<ParticleSystem>().digest]->Update();
  systems[CHash::Hash<OCullSystem>().digest]->Update();
  systems[CHash::Hash<RenderSystem>().digest]->Update();
  systems[CHash::Hash<PhysicsDrawSystem>().digest]->Update();
  systems[CHash::Hash<RenderSystem>().digest]->LateUpdate();
}

void World::Update()
{
  for ( auto hash : update_sequence )
    systems[hash]->Update();
}

void World::LateUpdate()
{
  for ( auto hash : late_update_sequence )
    systems[hash]->LateUpdate();
}

void World::Save( std::string filename )
{
  JsonFile file;
  file.StartWriter( filename ).WriteEntities( *em ).EndWriter();
}

void World::Load( std::string filename )
{
  JsonFile file;
  file.StartReader( filename ).LoadEntities( *em ).EndReader();
}
}
