#pragma once
#include <string>
#include "Core/TypeAlias.h"

namespace DeltaEngine
{
  struct EntityName
  {
    std::string name{};
  };

  struct Parent
  {
    size_t p_id{u64_max};
  };
}
