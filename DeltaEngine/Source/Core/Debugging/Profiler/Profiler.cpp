#include "Profiler.h"
#include "Core/Debugging/Logger/Log.h"
namespace DeltaEngine
{
  void Profiler::FrameStart()
  {
    m_prev = m_start = HighResClock::now();
  }

  void Profiler::FrameEnd()
  {
    TimePoint end = HighResClock::now();

    m_delta[m_index] = end - m_start;

    m_index = 1 - m_index;
    m_buffer[m_index].clear();
  }

  void Profiler::Record(std::string_view str)
  {
    TimePoint current = HighResClock::now();
    Nanoseconds delta = current - m_prev;
    m_buffer[m_index].push_back( { std::string( str ), delta } );
    m_prev = current;
  }


  void Profiler::Print()
  {
    DeltaEngine_CORE_INFO( "Profiler Status Start" );
    const Nanoseconds &delta_ref = m_delta[1 - m_index];
    for (const auto& entries : m_buffer[1 - m_index])
    {
      double percentage = ((entries.delta.count() * 1e-9) / (delta_ref.count() * 1e-9)) * 100.0;
      DeltaEngine_CORE_INFO( "Entry - {}, Delta: {}ms, Percentage: {}%", 
                             entries.name, 
                             static_cast<float>(entries.delta.count() * 1e-6), 
                             percentage );
    }
    DeltaEngine_CORE_INFO( "Profiler Status End");
  }


}