#pragma once

#include "World.h"

namespace DeltaEngine
{
  class ECSModule
  {
    std::vector<std::unique_ptr<World>> m_worlds;

  public:
    ECSModule()
    {
      m_worlds.push_back(std::make_unique<World>());
    }

    World& GetWorld()
    {
      return *m_worlds[0]; // default for now
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
