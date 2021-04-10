#include "Cutscenes.h"

namespace DeltaEngine
{
  float Cutscene::introTimings[11] = { 4,3.5,1.5,2,4.5,2,4,2,4,2,3 };
  float Cutscene::endinTimings[1] = { 4 };
  float Cutscene::timer = 0;
  int Cutscene::frame = -1;
  bool Cutscene::intro = 1;
}
