#pragma once

#include <typeinfo>
#include <typeindex>
#include <bitset>
#include <cassert>
#include <array>
#include <queue>
#include <set>
#include <memory>
#include <unordered_map>

namespace DeltaEngine
{
  using Index = size_t;
  using Size = size_t;
  using Entity = unsigned int;
  const Entity MAX_ENTITIES = 10000;
  using ComponentType = unsigned char;
  const ComponentType MAX_COMPONENTS = 64;
  using BitSignature = std::bitset<MAX_COMPONENTS>;
} // namespace DeltaEngine
