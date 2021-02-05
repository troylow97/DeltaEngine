/**********************************************************************************
* \file   Asset.h
* \brief  This file contains the definition of Asset
*         Asset is a wrapper container over the actual resource of type T
*
*         Asset is a custom pointer wrapper to the AssetData stored in AssetGroup
*         
*         This custom pointer wrapper contains the following functionality:
*         - Switching to Fallback Data
*         - Reference tracking
*         - Lifetime tracking
*         - State tracking
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "AssetKey.h"
#include "AssetEnum.h"
#include "AssetGroup.h"

namespace DeltaEngine
{
  template <typename T1, typename T2 = T1>
  class Asset
  {
  public:
    // Default
    explicit Asset() = default;

    // Copy
    Asset(const Asset& rhs);
    Asset& operator=(const Asset& rhs);

    // Move
    Asset(Asset&& rhs) noexcept;
    Asset& operator=(Asset&& rhs) noexcept;

    // Destructor
    ~Asset();

    bool operator==(const Asset& rhs) const;
    bool operator!=(const Asset& rhs) const;
    operator bool();
    operator T2*();
    T2& operator*();
    T2* operator->();

    AssetKey Key() const;
    AssetState State();

  private:
    Asset(AssetGroup<T1>* group, AssetKey key);
    void Acquire();

    friend AssetGroup<T1>;
    AssetGroup<T1>* m_group{nullptr};
    T1* m_data{nullptr};
    AssetKey m_key{};
    size_t m_timestamp{0};
    AssetState m_state{AssetState::Final};
  };
} // namespace DeltaEngine

#include "Asset.inl"
