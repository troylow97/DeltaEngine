#pragma once
#include "DE_API.h"

namespace DeltaEngine
{

class DE_API SystemBase
{
protected:
  class EntityManager &em;

public:
  class Query e_query;

  explicit SystemBase( EntityManager &ref ) : em { ref }
  {}

  virtual void Init() {}

  virtual void Deinit() {}

  virtual void Update() = 0;

  virtual void LateUpdate() = 0;

  virtual ~SystemBase() = default;
};

} // namespace DeltaEngine