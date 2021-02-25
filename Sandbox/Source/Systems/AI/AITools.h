/**********************************************************************************
* \file   AITools.h
* \brief  The file contains the code for AI Tools, minor gameplay logic.
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
  namespace AITools
  {
    float Distance_X_BetweenTwoEntities(EntityID& id1, EntityID& id2);
    float Distance_Y_BetweenTwoEntities(EntityID& id1, EntityID& id2);
    float Distance_X_BetweenEntityAndPoint(EntityID& id1, Vector2& point);
    float Distance_Y_BetweenEntityAndPoint(EntityID& id1, Vector2& point);
    float Distance_Total_BetweenTwoEntities(EntityID& id1, EntityID& id2);
    void FaceLeft(EntityID& id);
    void FaceRight(EntityID& id);
    bool isFacingLeft(EntityID& id);
    bool isFacingRight(EntityID& id);
    bool isFacingEachOther(EntityID& id1, EntityID& id2);
    void FaceEntity(EntityID& id1, EntityID& id2);
    void MoveRight(EntityID& id);
    void MoveLeft(EntityID& id);
    void MoveStop(EntityID& id);
    bool EntityisWithinDetectionRange(EntityID& id1, EntityID& id2, float x, float y);
    bool EntityisWithinDetectionRange(EntityID& id1, Vector2& point, float x, float y);
    bool EntityisOnTheRight(EntityID& id1, EntityID& id2);
    bool EntityisOnTheLeft(EntityID& id1, EntityID& id2);
    bool EntityisOnTop(EntityID& id1, EntityID& id2);
    bool EntityisOnBot(EntityID& id1, EntityID& id2);
    bool PointisOnTheRight(EntityID& id1, Vector2& point);
    bool PointisOnTheLeft(EntityID& id1, Vector2& point);
    bool PointisOnTop(EntityID& id1, Vector2& point);
    bool PointisOnBot(EntityID& id1, Vector2& point);
    void MoveTowardsEntityInX(EntityID& id1, EntityID& id2);
    void MoveTowardsEntityInX(EntityID& id1, float& pointX);
    void MoveTowardsEntity(EntityID& id1, EntityID& id2);
    void BulletTowardsEntity(EntityID& bullet, EntityID& entity);
    void MoveTowardsPoint(EntityID& id1, Vector2& point);
    void MoveTowardsPoint(EntityID& id1, float& x);
    void FlyTowardsPoint(EntityID& id1,const Vector2 point);
    bool EntityisAtPoint(EntityID& id1, Vector2& point);
    bool EntityisAtPointInX(EntityID& id1, float x, float tolerance);
    bool EntityisAtPoint(EntityID& id1, Vector2& point, float tolerance);
  }
}
