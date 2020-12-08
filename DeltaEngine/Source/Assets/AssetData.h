/**********************************************************************************
* \file   AssetData.h
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
#include "Core/Debugging/Assert.h"

namespace DeltaEngine
{
  namespace Internal
  {
    template <typename T>
    void safe_delete(T* data)
    {
      static_assert( sizeof(T) > 0 );
      delete data;
    }
  } // namespace Internal

  template <typename T1>
  struct AssetData
  {
    T1* data{nullptr};
    AssetState state{AssetState::Mutable};
    AssetLifetime lifetime{AssetLifetime::Managed};
    size_t reference_count{0};

    explicit AssetData() = default;

    // No Copy
    AssetData(const AssetData&) = delete;
    AssetData& operator=(const AssetData&) = delete;

    // Move Construction, no move assignment
    AssetData(AssetData&& rhs) noexcept : data{rhs.data},
                                          state{rhs.state},
                                          lifetime{rhs.lifetime},
                                          reference_count{rhs.reference_count}
    {
      rhs.data = nullptr;
      rhs.reference_count = 0;
    }

    AssetData& operator=(AssetData&&) = delete;

    ~AssetData()
    {
      ASSERT_ERROR(reference_count == 0, "AssetData: Destroying data while referenced")
      if (data)
        Internal::safe_delete(data);
    }
  };
} // namespace DeltaEngine
