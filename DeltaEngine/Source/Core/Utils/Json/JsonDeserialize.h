/**********************************************************************************
* \file   JsonDeserialize.h
* \brief  This file contains the definition of Json deserialization using RTTR
*         The original source is modified to support the engine custom data type
*
* \author Tan, Tong Wee, 30% Code Contribution
*         70% Code from official RTTR
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
