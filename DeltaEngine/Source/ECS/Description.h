#pragma once

namespace DeltaEngine
{

struct ComponentMeta;

struct Description
{
  struct Details
  {
    const ComponentMeta* meta;
    size_t offset;
  };
  std::vector<Details> metalist;
  size_t capacity { 0 };
};

} // namespace DeltaEngine