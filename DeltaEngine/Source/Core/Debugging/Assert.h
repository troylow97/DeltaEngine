/**********************************************************************************
* \file   Assert.h
* \brief  This file contains the definition of the engine custom assert operation
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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
