#include "OCullSystem.h"
#include "Render/Camera.h"
#include "Components/Components.h"
#include "Physics/Collision.h"
#include "ImGui/Panels/IPanel.h"
#ifdef DE_EDITOR
#include "ImGui/Editor.h"
#endif

namespace DeltaEngine
{
  void OCullSystem::Update()
  {

    Vector3 max {};
    Vector3 min {};
    Vector2 c_size {};
    Vector2 c_center {};


#ifdef DE_EDITOR
    if ( Editor::Instance().m_panels[8]->IsActive())
    {
      max = Camera::editorCamera->Max( Camera::editorCameraTransform );
      min = Camera::editorCamera->Min( Camera::editorCameraTransform );
      c_center = { Camera::editorCameraTransform.position.x,Camera::editorCameraTransform.position.y };
    }
    else
#endif
    {
      auto t = em.GetComponent<Transform>( { 0 } );
      max = Camera::allCameras[0]->Max( t );
      min = Camera::allCameras[0]->Min( t );
      c_center = { t.position.x, t.position.y };
    }
    c_size = { (max.x - min.x), (max.y - min.y )};

    em.ForEach( [&]( Transform &t, Image &i, Renderer2D &r )
    {
      Vector2 e_t { t.position.x, t.position.y };
      if ( CollisionIntersection_RectRect_Static( e_t, i.GetWorldSize(), c_center, c_size ) )
        r.m_Active = true;
      else
        r.m_Active = false;
    } );
  }

  void OCullSystem::LateUpdate()
  {



  }
}
