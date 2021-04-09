#pragma once

namespace DeltaEngine
{
  struct Cutscene
  {
    static char introTimings[11];
    static char endinTimings[11];

    static float timer;
    static int frame;
    static bool intro;
  };
}
