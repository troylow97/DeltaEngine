#pragma once
#include "DE_API.h"

namespace DeltaEngine
{

  struct EntityID;

  class DE_API Query
  {
    std::vector<MetaHash> required;
    std::vector<MetaHash> excluded;

    size_t required_matcher_hash{0};
    size_t excluded_matcher_hash{0};

    friend class EntityManager;

    void initialize()
    {
      required.clear();
      excluded.clear();
      required_matcher_hash = 0;
      excluded_matcher_hash = 0;
    }

  public:

    template <typename... C>
    Query &with()
    {
      (required.push_back(Metatype::build_hash<C>()), ...);
      return *this;
    }

    template <typename... C>
    Query &exclude()
    {
      (excluded.push_back(Metatype::build_hash<C>()), ...);
      return *this;
    }

    Query &clear()
    {
      initialize();
      return *this;
    }

    Query &build()
    {

      auto hash_remove_entityID = [](const MetaHash &type) {
        return type == Metatype::build_hash<EntityID>();
      };

      required.erase(std::remove_if(required.begin(), required.end(), hash_remove_entityID), required.end());
      excluded.erase(std::remove_if(excluded.begin(), excluded.end(), hash_remove_entityID), excluded.end());

      auto hash_compare = [](const MetaHash &lhs, const MetaHash &rhs) {
        return lhs.digest < rhs.digest;
      };

      std::sort(required.begin(), required.end(), hash_compare);
      std::sort(excluded.begin(), excluded.end(), hash_compare);

      auto hash_matcher = [](const std::vector<MetaHash> &hashes) {
        size_t matcher{0};

        for (auto hash : hashes)
          matcher |= hash.matcher;

        return matcher;
      };

      required_matcher_hash = hash_matcher(required);
      excluded_matcher_hash = hash_matcher(excluded);

      return *this;
    }
  };

} // namespace DeltaEngine
