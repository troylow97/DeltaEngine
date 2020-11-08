#pragma once
#include "Event/Event.h"

namespace DeltaEngine
{

class Layer
{
public:
  Layer( const std::string &name = "Layer" );
  virtual ~Layer() = default;

  virtual void OnAttach()
  {}
  virtual void OnDetach()
  {}
  virtual void OnUpdate()
  {}
  virtual void OnImGuiRender()
  {}
  virtual void OnEvent(Event* e)
  {}

  const std::string &GetName() const
  {
    return m_DebugName;
  }
protected:
#pragma warning(disable:4251)
  std::string m_DebugName;
#pragma warning(default:4251)
};

}