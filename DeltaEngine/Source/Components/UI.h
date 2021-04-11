/**********************************************************************************
* \file   UI.h
* \brief  The file contains components related for UIType
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

namespace DeltaEngine
{

enum class GUIType : unsigned
{
  Canvas,
  Text,
  Image,
  Button,
  Toggle,
  Slider,
  Cursor
};

struct GUI
{
  GUIType type {GUIType::Canvas};
  unsigned screen { 0 };
};

struct Button
{
  std::string on_hover {};
  std::string on_click {};
  std::string on_exit {};
};

struct Toggle
{
  std::string on_change {};
  bool value { false };
};

struct Slider
{
  std::string on_change {};
  unsigned fill_entity {u32_max};
  unsigned handle_entity { u32_max };
  float min { 0 };
  float max { 0 };
  float value { 0 };
  bool selected { false };
};

struct Cursor
{
  bool visible;
  bool enabled;
};

}
