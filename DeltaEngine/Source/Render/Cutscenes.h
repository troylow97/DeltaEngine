#pragma once
#include <string>

namespace DeltaEngine
{
  struct Cutscene
  {
    static float introTimings[11];
    static float endinTimings[7];

    static float timer;
    static int frame;
    static int totalFrames;
    static std::string cutscenePrefix;
    static bool intro;
  };
}
