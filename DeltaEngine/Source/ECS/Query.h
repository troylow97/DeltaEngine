#pragma once
#include "DE_API.h"

namespace DeltaEngine
{

struct EntityID;

class DE_API Query
{
  std::vector<MetaHash> m_required;
  std::vector<MetaHash> m_excluded;

  size_t m_required_matcher_hash { 0 };
  size_t m_excluded_matcher_hash { 0 };

  friend class EntityManager;

  void Initialize()
  {
    m_required.clear();
    m_excluded.clear();
    m_required_matcher_hash = 0;
    m_excluded_matcher_hash = 0;
  }

public:

  template <typename... C>
  Query &With()
  {
    ( m_required.push_back( Metatype::BuildHash<C>() ), ... );
    return *this;
  }

  Query &With( MetaHash hash )
  {
    m_required.push_back( hash );
    return *this;
  }

  template <typename... C>
  Query &Exclude()
  {
    ( m_excluded.push_back( Metatype::BuildHash<C>() ), ... );
    return *this;
  }

  Query &Exclude( MetaHash hash )
  {
    m_excluded.push_back( hash );
    return *this;
  }

  Query &Clear()
  {
    Initialize();
    return *this;
  }

  Query &Build()
  {

    auto hash_remove_entityID = []( const MetaHash &type )
    {
      return type == Metatype::BuildHash<EntityID>();
    };

    m_required.erase( std::remove_if( m_required.begin(), m_required.end(), hash_remove_entityID ), m_required.end() );
    m_excluded.erase( std::remove_if( m_excluded.begin(), m_excluded.end(), hash_remove_entityID ), m_excluded.end() );

    auto hash_compare = []( const MetaHash &lhs, const MetaHash &rhs )
    {
      return lhs.digest < rhs.digest;
    };

    std::sort( m_required.begin(), m_required.end(), hash_compare );
    std::sort( m_excluded.begin(), m_excluded.end(), hash_compare );

    auto hash_matcher = []( const std::vector<MetaHash> &hashes )
    {
      size_t matcher { 0 };

      for ( const auto hash : hashes )
        matcher |= hash.matcher;

      return matcher;
    };

    m_required_matcher_hash = hash_matcher( m_required );
    m_excluded_matcher_hash = hash_matcher( m_excluded );

    return *this;
  }
};

} // namespace DeltaEngine
