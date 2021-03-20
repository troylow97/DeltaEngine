/**********************************************************************************
* \file   Reflect.h
* \brief  The file contains the function to reflect and serialise data
* \author Chin, Clara,   25% Code Contribution
* \author Low, Troy,     25% Code Contribution
* \author Ong, Graeme,   25% Code Contribution
* \author Tan, Tong Wee, 25% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <rttr/type.h>

namespace DeltaEngine
{
  struct EntityID;
  class EntityManager;
}

namespace DeltaEngine::RT_Reflect
{
  rttr::type RT_Checker(size_t bits);

  rttr::instance RT_Getter(EntityManager& em, EntityID& id, size_t bits);

  void RT_Setter(EntityManager& em, EntityID id, size_t bits);

  void RT_Destroy(EntityManager& em, EntityID id, size_t bits);

  void SerializeType(const std::string& str, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer, void* ptr);

  void DeserializeType(const std::string& str, EntityManager& em, EntityID id, rttr::variant var, unsigned p_adj);
}
