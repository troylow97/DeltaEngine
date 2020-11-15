#define RAPIDJSON_HAS_STDSTRING 1

#include "JsonFile.h"
#include "JsonSerialize.h"
#include "JsonDeserialize.h"
#include <string>
#include "Core/Debugging/Assert.h"
using namespace rttr;
using namespace rapidjson;

namespace DeltaEngine
{
  JsonFile& JsonFile::StartWriter(std::string filename)
  {
    ASSERT_ERROR(m_fp == nullptr, "JsonFile: Another file is currently opened, close current operation")
    fopen_s(&m_fp, filename.c_str(), "wb");
    m_buffer = new char[65536]{};
    m_wstream = new FileWriteStream(m_fp, m_buffer, 65536);
    m_writer = new PrettyWriter<FileWriteStream>(*m_wstream);
    return *this;
  }

  JsonFile& JsonFile::EndWriter()
  {
    delete m_writer;
    delete m_wstream;
    delete m_buffer;
    fclose(m_fp);
    m_fp = nullptr;
    return *this;
  }

  JsonFile& JsonFile::StartObject()
  {
    m_writer->StartObject();
    return *this;
  }

  JsonFile& JsonFile::EndObject()
  {
    m_writer->EndObject();
    return *this;
  }

  JsonFile& JsonFile::StartArray()
  {
    m_writer->StartArray();
    return *this;
  }

  JsonFile& JsonFile::EndArray()
  {
    m_writer->EndArray();
    return *this;
  }

  JsonFile& JsonFile::WriteKey(std::string str)
  {
    m_writer->String(str);
    return *this;
  }

  JsonFile& JsonFile::WriteObject(instance object)
  {
    Serialize::WriteObject(object, *m_writer);
    return *this;
  }

  JsonFile& JsonFile::WriteArray(const variant_sequential_view& view)
  {
    Serialize::WriteArray(view, *m_writer);
    return *this;
  }

  JsonFile& JsonFile::WriteAssociative(const variant_associative_view& view)
  {
    Serialize::WriteObject(view, *m_writer);
    return *this;
  }

  JsonFile& JsonFile::WriteEntities(class EntityManager& em)
  {
    Serialize::WriteEntities(em, *m_writer);
    return *this;
  }

  JsonFile& JsonFile::StartReader(std::string filename)
  {
    ASSERT_ERROR(m_fp == nullptr, "JsonFile: Another file is currently opened, close current operation")
    fopen_s(&m_fp, filename.c_str(), "rb");
    m_buffer = new char[65536];
    m_rstream = new FileReadStream(m_fp, m_buffer, 65536);
    m_doc = new Document();
    ASSERT_ERROR(!m_doc->ParseStream( *m_rstream ).HasParseError(), "JsonFile: Document parsing have failed");
    return *this;
  }

  JsonFile& JsonFile::EndReader()
  {
    delete m_doc;
    delete m_rstream;
    delete m_buffer;
    fclose(m_fp);
    m_fp = nullptr;
    return *this;
  }

  JsonFile& JsonFile::LoadArray(variant_sequential_view& view, Value& json_array_value)
  {
    Deserialize::ReadArray(view, json_array_value);
    return *this;
  }

  JsonFile& JsonFile::LoadAssociative(variant_associative_view& view, Value& json_array_value)
  {
    Deserialize::ReadAssociative(view, json_array_value);
    return *this;
  }

  JsonFile& JsonFile::LoadEntities(class EntityManager& em)
  {
    Value::MemberIterator it = m_doc->FindMember("Entities");
    if (it != m_doc->MemberEnd())
      Deserialize::ReadEntities(em, it);
    return *this;
  }

  JsonFile& JsonFile::LoadObject(instance object)
  {
    Deserialize::ReadObject(object, *m_doc);
    return *this;
  }
}
