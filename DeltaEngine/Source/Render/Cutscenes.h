#pragma once

namespace DeltaEngine
{
  struct Cutscene
  {
    static float introTimings[11];
    static float endinTimings[1];

    static float timer;
    static int frame;
    static bool intro;
  };
}
