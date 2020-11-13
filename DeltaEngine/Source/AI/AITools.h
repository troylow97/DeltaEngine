#pragma once
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
	namespace AITools
	{
		float Distance_X_BetweenTwoEntities(EntityID& id1, EntityID& id2);
		float Distance_Y_BetweenTwoEntities(EntityID& id1, EntityID& id2);
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
		bool EntityisOnTheRight(EntityID& id1, EntityID& id2);
		bool EntityisOnTheLeft(EntityID& id1, EntityID& id2);
		bool EntityisOnTop(EntityID& id1, EntityID& id2);
		bool EntityisOnBot(EntityID& id1, EntityID& id2);
		void MoveTowardsEntityInX(EntityID& id1, EntityID& id2);
		void MoveTowardsEntity(EntityID& id1, EntityID& id2);
		void MoveTowardsPoint(EntityID& id1, Vector2& point);
		bool EntityisAtPoint(EntityID& id1, Vector2& point);
	}
}
