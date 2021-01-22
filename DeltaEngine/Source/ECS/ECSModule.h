/**********************************************************************************
* \file   ECSModule.h
* \brief  This file contains the implementation of the ECS interface
*
* \author Tan, Tong Wee, X% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "World.h"

namespace DeltaEngine
{
  class ECSModule 
  // can be the factory class itself
  // should always use unique_ptr, will prevent duplicate stuff by accident
  {
    std::vector<std::unique_ptr<World>> m_worlds;

  public:
    ECSModule()
    {
      DeltaEngine_CORE_INFO("Initializing ECS...");
      DeltaEngine_CORE_INFO("Creating default world");
      m_worlds.push_back(std::make_unique<World>());
      DeltaEngine_CORE_INFO("Initializing ECS successful");
      //m_worlds.push_back(std::make_unique<World>());
    }

    ~ECSModule()
    {
      DeltaEngine_CORE_INFO("Shutting down ECS...");
      for (auto& world : m_worlds)
        world->ShutdownSystems();
      DeltaEngine_CORE_INFO("Shutting down ECS successful");
    }

    World& GetWorld()
    {
      return *m_worlds[0]; // default for now
    }

    void GameWorld() // the factor class
    {
    //    std::map<int, ICloneWorld*>::iterator it = m_worlds[1];
      //WorldFactory worldfactory;
      //worldfactory.addPrototype(1, &m_worlds[1]);
    }
    
    World& GetGameWorld()
    {
    }

    // Multiple World Usage in the future
    // Get Entities Component Data from other world
    // Copy Entities Component Data to another world
    // Move Entities Component Data to another world

    // Rendering World
    // Simulation World
  };
} // namespace DeltaEngine

#define DEFINE_SYSTEM(TYPE, ...)                                                  \
  struct TYPE : public DeltaEngine::SystemBase                                    \
  {                                                                               \
    explicit TYPE(DeltaEngine::EntityManager &ref) : DeltaEngine::SystemBase(ref) \
    {                                                                             \
      DeltaEngine::Query q;                                                       \
      q.With<struct __VA_ARGS__>();                                               \
      e_query = q;                                                                \
    }

#define END_DEFINE_SYSTEM(TYPE) \
  }                             \
  ;
