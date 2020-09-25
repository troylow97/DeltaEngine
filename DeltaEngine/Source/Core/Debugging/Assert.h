#pragma once

#include "Logger/Log.h"

#ifdef DE_DEBUG

#define ASSERT(e)   \
  if (e)            \
  {                 \
  }                 \
  else              \
  {                 \
    __debugbreak(); \
  }

#define ASSERT_ERROR(e, ...)             \
  if (e)                                 \
  {                                      \
  }                                      \
  else                                   \
  {                                      \
    DeltaEngine_CORE_ERROR(__VA_ARGS__); \
    __debugbreak();                      \
  }
#else

#define ASSERT(e) ((void)(0))

#define ASSERT_ERROR(e, ...)             \
  if (e)                                 \
  {                                      \
  }                                      \
  else                                   \
  {                                      \
    DeltaEngine_CORE_ERROR(__VA_ARGS__); \
  }

#endif