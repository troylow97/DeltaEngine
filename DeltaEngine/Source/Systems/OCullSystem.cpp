/**********************************************************************************
* \file   OCullSystem.cpp
* \brief  The file contains implementation of OCullSystem.
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
  static bool _enable;

  void OCullSystem::Update()
  {
    if (_enable)
    {
      Vector3 max{};
      Vector3 min{};
      Vector2 c_size{};
      Vector2 c_center{};


#ifdef DE_EDITOR
			if (Editor::Instance().m_panels[8]->IsActive())
			{
				max = Camera::editorCamera->Max(Camera::editorCameraTransform);
				min = Camera::editorCamera->Min(Camera::editorCameraTransform);
				c_center = { Camera::editorCameraTransform.position.x,Camera::editorCameraTransform.position.y };
			}
			else
#endif
      {
        if (Camera::allCameras.size())
        {
          const auto& t = em.GetComponent<Transform>({0});
          max = Camera::allCameras[0]->Max(t);
          min = Camera::allCameras[0]->Min(t);
          c_center = {t.position.x, t.position.y};
        }
      }
      c_size = {(max.x - min.x) * 10, (max.y - min.y) * 10};

      Query q;
      q.Exclude<GUI>();

      em.ForEach(q, [&](Transform& t, Image& i, Renderer2D& r, EntityType& et)
      {
        Vector2 e_t{t.position.x, t.position.y};
        if (CollisionIntersection_RectRect_Static(e_t, i.GetWorldSize(), c_center, c_size))
          r.m_Active = true;
        else
          r.m_Active = false;

        em.ForEach([&](EntityID& id, EntityType& et, Transform& t, Image& im, Animator& anim, Renderer2D& r2d)
        {
          if (et.type == EntityCategory::E_PLAYER)
          {
            auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(id);
            if (a.SMGAttack)
            {
              r2d.m_Active = false;
              EnableShootingAnimation = true;
            }
            else
            {
              r2d.m_Active = true;
              EnableShootingAnimation = false;
            }
          }
          if (et.type == EntityCategory::E_PLAYER_BODYPART_ROTATABLE || et.type == EntityCategory::E_PLAYER_BODYPART)
          {
            //auto& player_bodypart_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id);

            if (EnableShootingAnimation)
              r2d.m_Active = true;
            else
              r2d.m_Active = false;
          }
        });
      });
    }
  }

  void OCullSystem::LateUpdate()
  {
  }

  void OCullSystem::Enable(bool b)
  {
    _enable = b;
  }
}
