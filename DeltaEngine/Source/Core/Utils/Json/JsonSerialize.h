/**********************************************************************************
* \file   JsonSerialize.h
* \brief  This file contains the definition for Json Serialization using RTTR
*         The original source is modified to support the engine custom data type
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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
  // To be used for general saving
  void WriteObject(rttr::instance object, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);
  void WriteArray(const rttr::variant_sequential_view& view,
                  rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);
  void WriteAssociative(const rttr::variant_associative_view& view,
                        rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

  // To be used for saving entities
  void WriteEntities(class EntityManager& em, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);
}
