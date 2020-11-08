#pragma once

namespace DeltaEngine
{

class SystemBase
{
protected:
  class EntityManager &em;

public:
  class Query e_query;

  explicit SystemBase( EntityManager &ref ) : em { ref }
  {}

  virtual void Initialize() {}

  virtual void Update() = 0;

  virtual void LateUpdate() = 0;

  virtual void Shutdown() {}

  virtual ~SystemBase() = default;
};

} // namespace DeltaEngine