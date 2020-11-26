#pragma once

struct Lifespan
{
  float Timer;

  Lifespan() :
    Timer{1.0f}
  {}

  Lifespan(float time) :
      Timer{ time }
  {}
};
