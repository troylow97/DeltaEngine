#define RAPIDJSON_HAS_STDSTRING 1

#include "JsonFile.h"
#include "JsonSerialize.h"
#include "JsonDeserialize.h"
#include <string>
#include <iostream>

using namespace rttr;
using namespace rapidjson;

namespace DeltaEngine
{

JsonFile &JsonFile::StartWriter( std::string filename )
{
  assert( fp == nullptr );
  fopen_s( &fp, filename.c_str(), "wb" );
  buffer = new char[65536] {};
  wstream = new FileWriteStream( fp, buffer, 65536 );
  writer = new PrettyWriter<FileWriteStream>( *wstream );
  return *this;
}

JsonFile &JsonFile::EndWriter()
{
  delete writer;
  delete wstream;
  delete buffer;
  fclose( fp );
  fp = nullptr;
  return *this;
}

JsonFile &JsonFile::StartObject()
{
  writer->StartObject();
  return *this;
}

JsonFile &JsonFile::EndObject()
{
  writer->EndObject();
  return *this;
}

JsonFile &JsonFile::StartArray()
{
  writer->StartArray();
  return *this;
}

JsonFile &JsonFile::EndArray()
{
  writer->EndArray();
  return *this;
}

JsonFile &JsonFile::WriteKey( std::string str )
{
  writer->String( str );
  return *this;
}

JsonFile &JsonFile::Write( instance object )
{
  Serialize::Write( object, *writer );
  return *this;
}

JsonFile &JsonFile::WriteArray( const variant_sequential_view &view )
{
  Serialize::WriteArray( view, *writer );
  return *this;
}

JsonFile &JsonFile::WriteAssociativeContainer( const variant_associative_view &view )
{
  Serialize::Write( view, *writer );
  return *this;
}

JsonFile &JsonFile::WriteEntities( class EntityManager& em )
{
  Serialize::WriteEntities( em, *writer );
  return *this;
}

JsonFile &JsonFile::StartReader( std::string filename )
{
  assert( fp == nullptr );
  fopen_s( &fp, filename.c_str(), "rb" );
  buffer = new char[65536];
  rstream = new FileReadStream( fp, buffer, 65536 );
  doc = new Document();
  assert( !doc->ParseStream( *rstream ).HasParseError() );
  return *this;
}

JsonFile &JsonFile::EndReader()
{
  delete doc;
  delete rstream;
  delete buffer;
  fclose( fp );
  fp = nullptr;
  return *this;
}

JsonFile &JsonFile::LoadArray( variant_sequential_view &view, Value &json_array_value )
{
  Deserialize::ReadArray( view, json_array_value );
  return *this;
}

JsonFile &JsonFile::LoadAssociative( variant_associative_view &view, Value &json_array_value )
{
  Deserialize::ReadAssociative( view, json_array_value );
  return *this;
}

JsonFile &JsonFile::LoadEntities(class EntityManager& em)
{
  Value::MemberIterator it = doc->FindMember( "Entities" );
  if ( it != doc->MemberEnd() )
    Deserialize::ReadEntities( em, it );
  return *this;
}

JsonFile &JsonFile::LoadObject( instance object )
{
  Deserialize::ReadObject( object, *doc );
  return *this;
}

}