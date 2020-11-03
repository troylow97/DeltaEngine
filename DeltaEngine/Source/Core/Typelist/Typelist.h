#pragma once
#include "NullType.h"
#include "Details.h"
#include "Extractor.h"
#include "Concatenate.h"
#include "Find.h"
#include "Remove.h"
namespace DeltaEngine
{

  template <typename... Ts>
  struct Typelist
  {
    template <typename... AppendTypes>
    using Append = Typelist<Ts..., AppendTypes...>;

    template <typename InputList>
    using Extend = typename Types::Internal::Concatenate<Typelist<Ts...>, InputList>::type;

    template <template <typename> typename Predicate, typename NotFound = NullType>
    using Find = typename Types::Internal::Find<Predicate, NotFound, Ts...>::type;

    template <template <typename> typename Predicate>
    using Remove = typename Types::Internal::Remove<Predicate, Ts...>::type;

    template <size_t Index>
    using Extract = Types::Internal::Extractor<Index, Ts...>;

    template <typename U>
    static constexpr size_t index = Types::Internal::Details<U, Ts...>::index;

    static constexpr size_t size = sizeof...(Ts);

    static constexpr bool empty = size == 0 ? true : false;
  };

} // namespace DeltaEngine