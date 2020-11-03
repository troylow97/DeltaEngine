#pragma once

namespace DeltaEngine::Types::Internal
{
  template <typename U, typename... Us>
  struct Details;

  template <typename U, typename... Us>
  struct Details<U, U, Us...>
  {
    static constexpr size_t index = 0;
  };

  template <typename U, typename V, typename... Us>
  struct Details<U, V, Us...>
  {
    static constexpr size_t index = (Details<U, Us...>::index) + 1;
  };
} // namespace DeltaEngine::Types::Internal