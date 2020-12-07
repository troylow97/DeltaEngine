#include "GCameraSystem.h"
#include "Components/Components.h"
namespace DeltaEngine
{
void GCameraSystem::Update()
{

}

void GCameraSystem::LateUpdate()
{
  bool exist { false };
  Transform *c_t { nullptr };
  em.ForEach( [&]( Transform &t, Camera &c )
  {
    exist = true;
    c_t = &t;
  } );

  if ( exist )
    em.ForEach( [&]( Transform &t, Player &p )
  {
    c_t->position = t.position;
    c_t->position.y = 0.0f;
  } );
}
}