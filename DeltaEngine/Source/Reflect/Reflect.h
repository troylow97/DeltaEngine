#pragma once
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <rttr/type.h>

namespace DeltaEngine {
  struct EntityID;
  class EntityManager;
}

namespace DeltaEngine::RT_Reflect
{
rttr::type RT_Checker( size_t bits );

rttr::instance RT_Getter( EntityManager &em, EntityID &id, size_t bits );

void RT_Setter( EntityManager &em, EntityID id, size_t bits );

void RT_Destroy( EntityManager &em, EntityID id, size_t bits );

void SerializeType( const std::string &str, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer, void *ptr );

void DeserializeType( const std::string &str, EntityManager &em, EntityID id, rttr::variant var );

}
