#pragma once

namespace DeltaEngine
{
  template <typename... Ts>
  struct Typelist;
}

namespace DeltaEngine::Types::Internal
{
  template <typename List, typename InputList>
  struct Concatenate;

  template <typename... ListTypes, typename... InputListTypes>
  struct Concatenate<Typelist<ListTypes...>, Typelist<InputListTypes...>>
  {
    using type = Typelist<ListTypes..., InputListTypes...>;
  };
}