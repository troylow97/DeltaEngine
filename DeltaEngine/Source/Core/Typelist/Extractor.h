#pragma once

namespace DeltaEngine::Types::Internal
{
  template <size_t Index, typename... Ts>
  struct Extractor
  {
    static_assert(Index < sizeof...(Ts));

    template <size_t Curr, size_t Dest, typename... Us>
    struct Impl;

    template <size_t Curr, size_t Dest, typename U, typename... Us>
    struct Impl<Curr, Dest, U, Us...>
    {
      using type = typename Impl<Curr + 1, Dest, Us...>::type;
    };

    template <size_t Dest, typename U, typename... Us>
    struct Impl<Dest, Dest, U, Us...>
    {
      using type = U;
    };

    using type = typename Impl<0, Index, Ts...>::type;
  };
} // namespace DeltaEngine::Types::Internal
