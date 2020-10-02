#pragma once
#include "DE_API.h"

namespace DeltaEngine
{

  template <typename T>
  class DE_API ArrayView
  {
    T *data{nullptr};
    DataChunk *owner{nullptr};

  public:

    ArrayView() = default;

    ArrayView(T *data_ptr, DataChunk *data_owner)
    {
      data = data_ptr;
      owner = data_owner;
    }

    const DataChunk* chunk_owner() const
    {
      return owner;
    }



    bool is_valid() const
    {
      return data != nullptr;
    }

    unsigned size()
    {
      return owner->header.last;
    }

    const T &operator[](size_t index) const
    {
      return data[index];
    }

    T &operator[](size_t index)
    {
      return data[index];
    }

    T *begin()
    {
      return data;
    }

    T *end()
    {
      return data + owner->header.index;
    }
  };
} // namespace DeltaEngine