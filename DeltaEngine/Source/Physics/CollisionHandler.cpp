#include "CollisionHandler.h"

namespace DeltaEngine
{

void CollisionHandler::OnEnter(EntityID& id)
{
  m_enter.Invoke( id );
}
void CollisionHandler::OnStay(EntityID& id)
{
  m_stay.Invoke(id);
}
void CollisionHandler::OnExit(EntityID& id)
{

  m_exit.Invoke(id);
}

size_t CollisionHandler::RegisterOnEnter( Action<EntityID&> action )
{
  return m_enter.Subscribe( action );
}
size_t CollisionHandler::RegisterOnStay( Action<EntityID&> action )
{
  return m_stay.Subscribe( action );
}
size_t CollisionHandler::RegisterOnExit( Action<EntityID&> action )
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