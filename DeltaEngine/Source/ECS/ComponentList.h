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
    size_t capacity;
  };

  struct Input
  {
      int previousKey;
      int currentKey;
  };

} // namespace DeltaEngine