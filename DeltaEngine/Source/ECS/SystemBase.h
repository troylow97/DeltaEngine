#pragma once

namespace DeltaEngine
{

  class SystemBase
  {
  protected:
    class EntityManager &em;

  public:
    class Query e_query;

    explicit SystemBase(EntityManager &ref) : em{ref} {}

    virtual void update() = 0;

    virtual void late_update() = 0;

    virtual ~SystemBase() = default;
  };

} // namespace DeltaEngine