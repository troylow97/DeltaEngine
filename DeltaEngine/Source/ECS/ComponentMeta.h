#pragma once

#include "DE_API.h"
#include <array>
#include "Components/Components.h"

namespace DeltaEngine
{

struct ComponentMeta
{
  using Constructor = void( void * );
  using Destructor = void( void * );

  Constructor *constructor { nullptr };
  Destructor *destructor { nullptr };
  size_t bits { 0 };
  unsigned index { 0 };
  unsigned size { 0 };
  unsigned align { 0 };

  [[nodiscard]] bool IsEmpty() const;

  template <typename T>
  static constexpr ComponentMeta Build();

  static constexpr std::array<ComponentMeta, ComponentList::size> Init();

private:
  template <size_t Start, size_t End>
  static constexpr void InitLoop( std::array<ComponentMeta, ComponentList::size> &arr );

  static inline bool load { false };

public:
  template <typename T>
  static const ComponentMeta *GetComponentMeta();

  static const size_t BuildSignature( const std::vector<const ComponentMeta *> &meta_vec );
};

inline bool ComponentMeta::IsEmpty() const
{
  return align == 0;
}

template <typename T>
constexpr ComponentMeta ComponentMeta::Build()
{
  ComponentMeta cm;

  cm.constructor = []( void *ptr )
  {
    new( ptr ) T {};
  };

  cm.destructor = []( void *ptr )
  {
    static_cast<T *>( ptr )->~T();
  };

  cm.bits = 1ULL << ComponentList::index<T>;
  cm.index = ComponentList::index<T>;
  if constexpr ( std::is_empty_v<T> )
  {
    cm.align = 0;
    cm.size = 0;
  }
  else
  {
    cm.align = alignof( T );
    cm.size = sizeof( T );
  }

  return cm;
}

template <size_t Start, size_t End>
constexpr void ComponentMeta::InitLoop( std::array<ComponentMeta, ComponentList::size> &arr )
{
  arr[Start] = Build<ComponentList::Extract<Start>::type>();
  if constexpr ( Start != End )
    InitLoop<Start + 1, End>( arr );
}


constexpr std::array<ComponentMeta, ComponentList::size> ComponentMeta::Init()
{
  std::array<ComponentMeta, ComponentList::size> arr;
  InitLoop<0, ComponentList::size - 1>( arr );
  return arr;
}

template <typename T>
const ComponentMeta *ComponentMeta::GetComponentMeta()
{
  using T_Base = std::remove_const_t<std::remove_reference_t<T>>;

  static constexpr std::array<ComponentMeta, ComponentList::size> c_meta_vec = ComponentMeta::Init();
/*if (!load)
{
  constexpr auto arr = ComponentMeta::Init();
  static_assert( arr[0].bits == 1);
  for ( auto &meta : arr )
    c_meta_vec.push_back( meta );
  load = true;
}*/

  return &c_meta_vec[ComponentList::index<T_Base>];
}

inline const size_t ComponentMeta::BuildSignature( const std::vector<const ComponentMeta *> &meta_vec )
{
  size_t bits { 0 };

  for ( auto &meta : meta_vec )
    bits |= meta->bits;

  return bits;
}

} // namespace DeltaEngine