/**********************************************************************************
* \file   ArrayView.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "DataChunk.h"

namespace DeltaEngine
{
  class EntityManager;

  template <typename T>
  class ArrayView
  {
    T* m_data{nullptr};
    DataChunk* m_owner{nullptr};
  public:

    ArrayView() = default;

    ~ArrayView() = default;

    ArrayView(T* data_ptr, DataChunk* data_owner)
    {
      m_data = data_ptr;
      m_owner = data_owner;
    }

    [[nodiscard]] const DataChunk* ChunkOwner() const
    {
      return m_owner;
    }

    [[nodiscard]] bool IsValid() const
    {
      return m_data != nullptr;
    }

    [[nodiscard]] unsigned Size() const
    {
      return m_owner->header.index;
    }

    const T& operator[](size_t index) const
    {
      return m_data[index];
    }

    T& operator[](size_t index)
    {
      return m_data[index];
    }

    T* begin()
    {
      return m_data;
    }

    T* end()
    {
      return m_data + m_owner->header.index;
    }
  };
} // namespace DeltaEngine
