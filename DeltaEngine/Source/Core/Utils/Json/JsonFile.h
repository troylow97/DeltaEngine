#pragma once
#include <string>
#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rttr/type>

namespace DeltaEngine
{
class JsonFile
{
  char *buffer{nullptr};
  FILE *fp{nullptr};
  rapidjson::FileWriteStream* wstream{nullptr};
  rapidjson::FileReadStream *rstream{nullptr};
  rapidjson::PrettyWriter<rapidjson::FileWriteStream>* writer{nullptr};
  rapidjson::Document *doc{nullptr};

public:

  JsonFile &StartWriter(std::string filename);
  JsonFile &EndWriter();
  JsonFile &StartObject();
  JsonFile &EndObject();
  JsonFile &StartArray();
  JsonFile &EndArray();
  JsonFile &WriteKey( std::string str );
  JsonFile &Write(rttr::instance object);
  JsonFile &WriteArray( const rttr::variant_sequential_view &view );
  JsonFile &WriteAssociativeContainer( const rttr::variant_associative_view &view );
  JsonFile &WriteEntities(class EntityManager& em);

  JsonFile &StartReader( std::string filename );
  JsonFile &EndReader();
  JsonFile &LoadArray( rttr::variant_sequential_view &view, rapidjson::Value &json_array_value );
  JsonFile &LoadAssociative( rttr::variant_associative_view &view, rapidjson::Value &json_array_value );
  JsonFile &LoadEntities(class EntityManager& em);
  JsonFile &LoadObject(rttr::instance object);
};

}