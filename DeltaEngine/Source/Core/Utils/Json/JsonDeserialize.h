#pragma once
#include <rapidjson/document.h>
#include <rttr/type>

namespace DeltaEngine
{
  class EntityManager;
}

namespace DeltaEngine::Deserialize
{
  // To be used for general 
  void ReadObject(rttr::instance obj, rapidjson::Document& doc);
  void ReadArray(rttr::variant_sequential_view& view, rapidjson::Value& json_array_value);
  void ReadAssociative(rttr::variant_associative_view& view, rapidjson::Value& json_array_value);

  // To be used for loading entities into world
  void ReadEntities(EntityManager& em, rapidjson::Value::MemberIterator it);
}
