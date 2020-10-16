#pragma once

#include <unordered_map>
#include "DE_API.h"

namespace DeltaEngine
{

inline constexpr uint64_t hash_fnv1a( const char *key )
{
  uint64_t hash = 0xcbf29ce484222325;
  const uint64_t prime = 0x100000001b3;

  int i = 0;
  while ( key[i] )
  {
    const uint8_t value = key[i++];
    hash = hash ^ value;
    hash *= prime;
  }

  return hash;
}

struct MetaHash
{
  size_t digest { 0 };
  size_t matcher { 0 };

  bool operator==( const MetaHash &rhs ) const
  {
    return digest == rhs.digest;
  }

  template <typename T>
  static constexpr size_t Hash()
  {
    return hash_fnv1a( __FUNCSIG__ );
  }
};

struct Metatype
{
  using Constructor = void( void * );
  using Destructor = void( void * );

  MetaHash hash;

  Constructor *constructor { nullptr };
  Destructor *destructor { nullptr };
  unsigned size { 0 };
  unsigned align { 0 };

  [[nodiscard]] bool IsEmpty() const
  {
    return align == 0;
  }

  template <typename T>
  static constexpr MetaHash BuildHash()
  {
    using T_Base = std::remove_const_t<std::remove_reference_t<T>>;

    MetaHash hash;
    hash.digest = MetaHash::Hash<T_Base>();
    hash.matcher = 1ULL << hash.digest % 63ULL;
    return hash;
  }

  template <typename T>
  static constexpr Metatype Build()
  {
    Metatype meta;
    static_assert( BuildHash<T>().digest != 0 );
    meta.hash = BuildHash<T>();

    if constexpr ( std::is_empty_v<T> )
    {
      meta.align = 0;
      meta.size = 0;
    }
    else
    {
      meta.align = alignof( T );
      meta.size = sizeof( T );
    }

    meta.constructor = []( void *ptr )
    {
      new ( ptr ) T {};
    };
    meta.destructor = []( void *ptr )
    {
      static_cast<T *>( ptr )->~T();
    };

    return meta;
  }

  static inline std::unordered_map<size_t, Metatype> metatype_map;

  template <typename T>
  static const Metatype *GetMetatype()
  {
    constexpr size_t digest = Metatype::BuildHash<T>().digest;
    const auto type = Metatype::metatype_map.find( digest );
    if ( type == Metatype::metatype_map.end() )
    {
      constexpr Metatype new_type = Metatype::Build<T>();
      static_assert( new_type.hash.digest );
      Metatype::metatype_map[digest] = new_type;
    }

    return &Metatype::metatype_map[digest];
  }

  static size_t BuildSignature( const Metatype **types, size_t count )
  {
    size_t hash { 0 };

    for ( size_t i = 0; i < count; i++ )
      hash |= types[i]->hash.matcher;

    return hash;
  }

};



} // namespace DeltaEngine