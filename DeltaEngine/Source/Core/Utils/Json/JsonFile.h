/**********************************************************************************
* \file   JsonFile.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rttr/type>

namespace DeltaEngine
{
  class JsonFile
  {
    char* m_buffer{nullptr};
    FILE* m_fp{nullptr};
    rapidjson::FileWriteStream* m_wstream{nullptr};
    rapidjson::FileReadStream* m_rstream{nullptr};
    rapidjson::PrettyWriter<rapidjson::FileWriteStream>* m_writer{nullptr};
    rapidjson::Document* m_doc{nullptr};

  public:

    // Writing Operations
    JsonFile& StartWriter(std::string filename);
    JsonFile& EndWriter();
    JsonFile& StartObject();
    JsonFile& EndObject();
    JsonFile& StartArray();
    JsonFile& EndArray();
    JsonFile& WriteKey(std::string str);
    JsonFile& WriteObject(rttr::instance object);
    JsonFile& WriteArray( rttr::variant_sequential_view view);
    JsonFile& WriteAssociative( rttr::variant_associative_view view);
    JsonFile& WriteEntities(class EntityManager& em);

    // Reading Operations
    JsonFile& StartReader(std::string filename);
    JsonFile& EndReader();
    JsonFile& LoadArray(rttr::variant_sequential_view& view, rapidjson::Value& json_array_value);
    JsonFile& LoadAssociative(rttr::variant_associative_view& view, rapidjson::Value& json_array_value);
    JsonFile& LoadEntities(class EntityManager& em);
    JsonFile& LoadObject(rttr::instance object);
  };
}
