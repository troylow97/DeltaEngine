#pragma once

#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/filewritestream.h>
#include <rttr/type>

namespace DeltaEngine
{
class EntityManager;
}

namespace DeltaEngine::Serialize
{
void Write( rttr::instance object, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer );
void WriteArray( const rttr::variant_sequential_view &view, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer );
void WriteAssociative( const rttr::variant_associative_view &view, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer );
void WriteEntities( class DeltaEngine::EntityManager &em, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer );
} 