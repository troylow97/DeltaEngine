#pragma once

#include "World.h"

namespace DeltaEngine
{

  class ECSModule
  {
    std::vector<World> worlds;

  public:
    ECSModule()
    {
      worlds.push_back(World());
    }

    World &world()
    {
      return worlds[0]; // default for now
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
      q.with<struct __VA_ARGS__>();                                               \
      q.build();                                                                  \
      e_query = q;                                                                \
    }

#define END_DEFINE_SYSTEM(TYPE) \
  }                             \
  ;
