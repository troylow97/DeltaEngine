#pragma once

namespace DeltaEngine
{

  struct Metatype;

  struct ComponentList
  {
    struct ComponentMeta
    {
      const Metatype *type;
      size_t offset;
    };
    std::vector<ComponentMeta> metatypes;
    size_t capacity {0};
  };

} // namespace DeltaEngine