#include "CollisionHandler.h"

namespace DeltaEngine
{
std::map<std::string, CollisionCallbacks> CollisionHandler::m_handlers; //m_handlers["ReduceHP"] = &ReduceHP;

void CollisionHandler::OnEnter( Collider &col )
{
  m_enter.Invoke( col );
}
void CollisionHandler::OnStay( Collider &col )
{
  m_stay.Invoke( col );
}
void CollisionHandler::OnExit( Collider &col )
{

  m_exit.Invoke( col );
}

size_t CollisionHandler::RegisterOnEnter( Action<Collider &> action )
{
  return m_enter.Subscribe( action );
}
size_t CollisionHandler::RegisterOnStay( Action<Collider &> action )
{
  return m_stay.Subscribe( action );
}
size_t CollisionHandler::RegisterOnExit( Action<Collider &> action )
{
  return m_exit.Subscribe( action );
}

void CollisionHandler::UnregisterOnEnter( size_t id )
{
  m_enter.Unsubscribe( id );
}
void CollisionHandler::UnregisterOnStay( size_t id )
{
  m_stay.Unsubscribe( id );
}
void CollisionHandler::UnregisterOnExit( size_t id )
{
  m_exit.Unsubscribe( id );
}

} // namespace DeltaEngine