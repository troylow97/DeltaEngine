#pragma once
#include <string>

struct AI
{
  std::string key;
  std::string transition;

  AI() :
    key("idle"),
    transition("null")
  {
  }
};
