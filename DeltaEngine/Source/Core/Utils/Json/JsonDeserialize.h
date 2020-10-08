#pragma once
#include <rapidjson/document.h>
#include <rttr/type>

namespace DeltaEngine {
  class EntityManager;
}

namespace DeltaEngine::Deserialize
{
void ReadArray( rttr::variant_sequential_view &view, rapidjson::Value &json_array_value );
void ReadAssociative( rttr::variant_associative_view &view, rapidjson::Value &json_array_value );
void ReadEntities( DeltaEngine::EntityManager& em, rapidjson::Value::MemberIterator it );
void ReadObject( rttr::instance obj, rapidjson::Document &doc );
}
