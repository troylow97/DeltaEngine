/**********************************************************************************
* \file   AITools.cpp
* \brief  The file contains the code for AI Tools, minor gameplay logic.
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "AITools.h"

#include "Audio/AudioEngine.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
  namespace AITools
  {
    float Distance_X_BetweenTwoEntities(EntityID& id1, EntityID& id2)
    {
      if (env.pECS->GetWorld().GetEntityManager().HasComponent<Transform>(id1) &&
        env.pECS->GetWorld().GetEntityManager().HasComponent<Transform>(id2))
      {
        return std::abs(env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.x
          - env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id2).position.x);
      }

      DeltaEngine_CORE_ERROR("Distance_X_BetweenTwoEntities: Entity has no transform!");
      return 0.0f;
    }

    float Distance_Y_BetweenTwoEntities(EntityID& id1, EntityID& id2)
    {
      if (env.pECS->GetWorld().GetEntityManager().HasComponent<Transform>(id1) &&
        env.pECS->GetWorld().GetEntityManager().HasComponent<Transform>(id2))
      {
        return std::abs(env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.y
          - env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id2).position.y);
      }
      DeltaEngine_CORE_ERROR("Distance_Y_BetweenTwoEntities: Entity has no transform!");
      return 0.0f;
    }

    float Distance_X_BetweenEntityAndPoint(EntityID& id1, Vector2& point)
    {
      if (env.pECS->GetWorld().GetEntityManager().HasComponent<Transform>(id1))
      {
        return std::abs(env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.x
          - point.x);
      }

      DeltaEngine_CORE_ERROR("Distance_X_BetweenEntitiesAndPoint: Entity has no transform!");
      return 0.0f;
    }

    float Distance_Y_BetweenEntityAndPoint(EntityID& id1, Vector2& point)
    {
      if (env.pECS->GetWorld().GetEntityManager().HasComponent<Transform>(id1))
      {
        return std::abs(env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.y
          - point.y);
      }

      DeltaEngine_CORE_ERROR("Distance_X_BetweenEntitiesAndPoint: Entity has no transform!");
      return 0.0f;
    }

    float Distance_Total_BetweenTwoEntities(EntityID& id1, EntityID& id2)
    {
      if (env.pECS->GetWorld().GetEntityManager().HasComponent<Transform>(id1) &&
        env.pECS->GetWorld().GetEntityManager().HasComponent<Transform>(id2))
      {
        Vector2 temp = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position
          - env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id2).position;

        return std::abs(temp.Magnitude());
      }
      DeltaEngine_CORE_ERROR("Distance_Total_BetweenTwoEntities: Entity has no transform!");
      return 0.0f;
    }

    void FaceLeft(EntityID& id1)
    {
	  env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id1).m_FlipX = true;
    }

    void FaceRight(EntityID& id1)
    {
      env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id1).m_FlipX = false;
    }

    bool isFacingLeft(EntityID& id1)
    {
	  return env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id1).m_FlipX;
    }

    bool isFacingRight(EntityID& id1)
    {
	  return !env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id1).m_FlipX;
    }

    bool isFacingEachOther(EntityID& id1, EntityID& id2)
    {
      if (isFacingLeft(id1) && isFacingRight(id2) || isFacingRight(id1) && isFacingLeft(id2))
      {
        return true;
      }

      return false;
    }

    void MoveRight(EntityID& id)
    {
      auto& em = env.pECS->GetWorld().GetEntityManager();
      em.GetComponent<RigidBody>(id).Direction = Vector2::right();
      auto& trans = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id);
      auto& audio = em.GetComponent<AudioSource>(id);
      if (!audio.isStart && !audio.isPlayed)
      {
        if (em.HasComponent<Fiddler>(id))
        {
          audio.clip = "event:/Enemy/Fiddler/Fiddler Move";
          AudioEngine::SetEventVolume(audio.id, 0.1f);
          AudioEngine::AudioSourcePlay3DEvent(audio, { trans.position, {},{0,0,1},{0,1,0} });
        }
        else if (em.HasComponent<Serpentipede>(id))
        {
          auto& audio_to_play = em.GetComponent<AudioSource>(id);
          AudioEngine::SetEventVolume(audio.id, 0.1f);
          audio.clip = "event:/Enemy/Serpentipede/Serpentipede Moving";
          AudioEngine::AudioSourcePlay3DEvent(audio, { trans.position, {},{0,0,1},{0,1,0} });
        }
      }
    }

    void MoveLeft(EntityID& id)
    {
      auto& em = env.pECS->GetWorld().GetEntityManager();
      em.GetComponent<RigidBody>(id).Direction = Vector2::left();
      auto& trans = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id);
      auto& audio = em.GetComponent<AudioSource>(id);
      if (!audio.isStart && !audio.isPlayed)
      {
        if (em.HasComponent<Fiddler>(id))
        {
          audio.clip = "event:/Enemy/Fiddler/Fiddler Move";
          AudioEngine::SetEventVolume(audio.id, 0.1f);
          AudioEngine::AudioSourcePlay3DEvent(audio, { trans.position, {},{0,0,1},{0,1,0} });
        }
        else if (em.HasComponent<Serpentipede>(id))
        {
          AudioEngine::SetEventVolume(audio.id, 0.1f);
          audio.clip = "event:/Enemy/Serpentipede/Serpentipede Moving";
          AudioEngine::AudioSourcePlay3DEvent(audio, { trans.position, {},{0,0,1},{0,1,0} });
        }
      }
    }

    void MoveStop(EntityID& id)
    {
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = Vector2::zero();
    }

    void FaceEntity(EntityID& id1, EntityID& id2)
    {
      if (EntityisOnTheRight(id1, id2))
      {
        FaceRight(id1);
        return;
      }

      if (EntityisOnTheLeft(id1, id2))
      {
        FaceLeft(id1);
      }
    }

    bool EntityisWithinDetectionRange(EntityID& id1, EntityID& id2, float x, float y)
    {
      return ((Distance_X_BetweenTwoEntities(id1, id2) < x) && (Distance_Y_BetweenTwoEntities(id1, id2) < y));
    }

    bool EntityisWithinDetectionRange(EntityID& id1, Vector2& point, float x, float y)
    {
      return ((Distance_X_BetweenEntityAndPoint(id1, point) < x) && (Distance_Y_BetweenEntityAndPoint(id1, point) < y));
    }

    bool EntityisOnTheRight(EntityID& id1, EntityID& id2)
    {
      auto& id1_posX = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.x;
      auto& id2_posX = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id2).position.x;

      return (id2_posX > id1_posX);
    }

    bool EntityisOnTheLeft(EntityID& id1, EntityID& id2)
    {
      auto& id1_posX = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.x;
      auto& id2_posX = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id2).position.x;

      return (id2_posX < id1_posX);
    }

    bool EntityisOnTop(EntityID& id1, EntityID& id2)
    {
      auto& id1_posY = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.y;
      auto& id2_posY = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id2).position.y;

      return (id2_posY > id1_posY);
    }

    bool EntityisOnBot(EntityID& id1, EntityID& id2)
    {
      auto& id1_posY = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.y;
      auto& id2_posY = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id2).position.y;

      return (id2_posY < id1_posY);
    }

    bool PointisOnTheRight(EntityID& id1, Vector2& point)
    {
      return (point.x > env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.x);
    }

    bool PointisOnTheLeft(EntityID& id1, Vector2& point)
    {
      return (point.x < env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.x);
    }

    bool PointisOnTop(EntityID& id1, Vector2& point)
    {
      return (point.y > env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.y);
    }

    bool PointisOnBot(EntityID& id1, Vector2& point)
    {
      return (point.y < env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.y);
    }

    void MoveTowardsEntityInX(EntityID& id1, EntityID& id2)
    {
      if (EntityisOnTheRight(id1, id2))
      {
        MoveRight(id1);
        return;
      }
      if (EntityisOnTheLeft(id1, id2))
      {
        MoveRight(id2);
      }
    }

    void MoveTowardsEntityInX(EntityID& id1, float& pointX)
    {
      Vector2 pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position;
      if (pos.x > pointX)
          MoveLeft(id1);
      else
          MoveRight(id1);
    }

    void BulletTowardsEntity(EntityID& bullet, EntityID& entity)
    {
      const Vector2 pos{env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(bullet).position};
      const Vector2 pos2{env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(entity).position};

      if (std::abs(pos2.x - pos.x) < 0.3 && std::abs(pos2.y - pos.y) < 0.3)
      {
        const Vector2 diff{pos2 - pos};
        env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(bullet).AccumulatedForce = diff * 1500;
      }
    }

    void MoveTowardsPointInX(EntityID& id1, Vector2& point)
    {
      if (PointisOnTheRight(id1, point))
      {
        MoveRight(id1);
      }
      else if (PointisOnTheLeft(id1, point))
      {
        MoveLeft(id1);
      }
    }


    void FlyTowardsPoint(EntityID& id1,const Vector2 point)
    {
      const Vector2 pos{env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position};
      Vector2 diff{point - pos};
      const Vector2 temp = Normalise(diff);
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id1).Direction = temp;
    }

    bool EntityisAtPoint(EntityID& id1, Vector2& point)
    {
      if (env.pECS->GetWorld().GetEntityManager().HasComponent<Transform>(id1))
      {
        Vector2 pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position;
        const float xDiff = static_cast<float>(std::abs(point.x - pos.x));
        const float yDiff = static_cast<float>(std::abs(point.y - pos.y));

        if (xDiff < 0.1f && yDiff < 0.1f)
        {
          return true;
        }
      }
      return false;
    }

    bool EntityisAtPoint(EntityID& id1, Vector2& point, float tolerance)
    {
      if(env.pECS->GetWorld().GetEntityManager().IsEntityValid(id1))
      {
        Vector2 pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position;
        const float xDiff = static_cast<float>(std::abs(point.x - pos.x));
        const float yDiff = static_cast<float>(std::abs(point.y - pos.y));

        if (xDiff < tolerance && yDiff < tolerance)
        {
          return true;
        }
      }
      return false;
    }

    bool EntityisAtPointInX(EntityID& id1, float x, float tolerance)
    {
      float pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.x;
      if (std::abs(x - pos) < tolerance)
      {
        return true;
      }
      return false;
    }
  }
}
