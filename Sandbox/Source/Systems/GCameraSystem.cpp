#include "GCameraSystem.h"
#include "Components/Components.h"
namespace DeltaEngine
{
  void GCameraSystem::Update()
  {

  }

  void GCameraSystem::LateUpdate()
  {
    auto& c_t = em.GetComponent<Transform>( { 0 } );

    em.ForEach( [&]( Transform &t, Player &p )
    {
      c_t.position = t.position;
      c_t.position.y += 1.0f;
    } );
  }
}