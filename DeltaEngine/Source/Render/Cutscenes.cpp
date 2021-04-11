#include "Cutscenes.h"

namespace DeltaEngine
{
  float Cutscene::introTimings[11] = { 4,3.5,1.5,2,4.5,2,4,2,4,2,4 };
  float Cutscene::endinTimings[7] = { 4,5,5,3,4,5,4 };
  float Cutscene::timer = 0;
  int Cutscene::frame = -1;
  int Cutscene::totalFrames = 10;
  std::string Cutscene::cutscenePrefix = "Textures/INTRO_";
  bool Cutscene::intro = 1;
}
