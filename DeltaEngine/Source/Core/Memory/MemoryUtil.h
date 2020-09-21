#pragma once

#include "Core/TypeAlias.h"

namespace DeltaEngine
{

struct MemoryUtil
{
  static const u8 ALIGNMENT = 16;

  static void CheckAlignment( u8 alignment );

  static void *Alloc( const size mem_size, const u8 alignment);

  static void Free( ptr );
};

}