#include "MemoryUtil.h"
#include <stdexcept>
#include<vector>
namespace DeltaEngine
{

//void MemoryUtil::CheckAlignment( u8 alignment )
//{
//  const bool valid = alignment >= 8 && alignment <= 128 && ( alignment & ( alignment - 1 ) ) == 0;
//
//  if ( !valid )
//  {
//    throw std::invalid_argument
//    {
//      "Illegal alignment, must satisfy: alignment >= 8 && alignment <= 128 && ( alignment & ( alignment - 1 ) ) == 0"
//    };
//  }
//}
//
//void MemoryUtil::Free( vptr memory )
//{
//  const byte *aligned_memory = reinterpret_cast<byte *>( memory );
//  const ptrdiff adjustment = static_cast<ptrdiff>( aligned_memory[-1] );
//  const uptr aligned_address = reinterpret_cast<uptr>( memory );
//  const uptr base_address = aligned_address - adjustment;
//  vptr base_memory = reinterpret_cast<vptr>( base_address );
//  std::free( base_memory );
//}
//
//vptr MemoryUtil::Alloc( const size mem_size, const u8 alignment )
//{
//  CheckAlignment( alignment );
//
//  const size 
//}

}