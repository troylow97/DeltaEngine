/**********************************************************************************
* \file   Query.h
* \brief  This file contains the implementation of a Query object, a query object
*         is used by the EntityManager to include or exclude entities with certain
*         bit signatures
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace DeltaEngine
{
  class Query
  {
    size_t m_required_bits{0};
    size_t m_excluded_bits{0};


    friend class EntityManager;

    void Initialize()
    {
      m_required_bits = m_excluded_bits = 0;
    }

    template <typename C>
    void Required()
    {
      m_required_bits |= ComponentMeta::GetComponentMeta<C>()->bits;
    }

    template <typename C>
    void Excluded()
    {
      m_excluded_bits |= ComponentMeta::GetComponentMeta<C>()->bits;
    }

  public:

    template <typename... C>
    Query& With()
    {
      ( Required<C>(), ... );
      m_required_bits &= ~(1ULL);
      return *this;
    }

    Query& With(size_t bits)
    {
      m_required_bits |= bits;
      m_required_bits &= ~(1ULL);
      return *this;
    }

    template <typename... C>
    Query& Exclude()
    {
      ( Excluded<C>(), ...);
      m_excluded_bits &= ~(1ULL);
      return *this;
    }

    Query& Exclude(size_t bits)
    {
      m_excluded_bits |= bits;
      m_excluded_bits &= ~(1ULL);
      return *this;
    }

    Query& Clear()
    {
      Initialize();
      return *this;
    }

    Query operator+(const Query& rhs) const
    {
      Query tmp{*this};
      tmp.m_excluded_bits |= rhs.m_excluded_bits;
      tmp.m_required_bits |= rhs.m_required_bits;
      return tmp;
    }
  };
} // namespace DeltaEngine