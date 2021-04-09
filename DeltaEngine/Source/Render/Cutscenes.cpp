#include "Cutscenes.h"

namespace DeltaEngine
{
  char Cutscene::introTimings[11] = { 4,3,2,2,4,2,4,2,3,3,3 };
  char Cutscene::endinTimings[11] = { 4,3,2,2,4,2,4,2,3,3,3 };
  float Cutscene::timer = 0;
  int Cutscene::frame = -1;
  bool Cutscene::intro = 1;
}
