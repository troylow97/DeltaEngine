#include "AITools.h"
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
			if (env.pECS->GetWorld().GetEntityManager().HasComponent<Image>(id1))
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id1).m_FlipX = false;
			}

			DeltaEngine_CORE_ERROR("FaceLeft: Entity has no image!");
		}

		void FaceRight(EntityID& id1)
		{
			if (env.pECS->GetWorld().GetEntityManager().HasComponent<Image>(id1))
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id1).m_FlipX = true;
			}

			DeltaEngine_CORE_ERROR("FaceRight: Entity has no image!");

		}
		bool isFacingLeft(EntityID& id1)
		{
			return !env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id1).m_FlipX;
		}
		bool isFacingRight(EntityID& id1)
		{
			return env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id1).m_FlipX;
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
			env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = Vector2::right();
		}
		void MoveLeft(EntityID& id)
		{
			env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = Vector2::left();
		}
		void MoveStop(EntityID& id)
		{
			env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = Vector2::zero();
		}
		void FaceEntity(EntityID& id1, EntityID& id2)
		{
			if (isFacingLeft(id1) && EntityisOnTheRight(id1,id2))
			{
				FaceRight(id1);
				return;
			}

			if (isFacingRight(id1) && EntityisOnTheLeft(id1, id2))
			{
				FaceLeft(id1);
				return;
			}
		}
		bool EntityisWithinDetectionRange(EntityID& id1, EntityID& id2, float x, float y)
		{
			//auto& id2_posX = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id2).position.x;
			//auto& id1_posY = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position.y;

			if ((Distance_X_BetweenTwoEntities(id1, id2) < x) && (Distance_Y_BetweenTwoEntities(id1, id2) < y))
			{
				//if ((isFacingRight(id1) && id2_posX >= id1_posY) || (isFacingLeft(id1) && id2_posX <= id1_posY))
				{
					return true;
				}
			}

			return false;
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

		void MoveTowardsEntityInX(EntityID& id1, EntityID& id2)
		{
			if (EntityisOnTheRight(id1, id2))
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id1).Direction = Vector2::right();
				return;
			}
			if (EntityisOnTheLeft(id1, id2))
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id1).Direction = Vector2::left();
				return;
			}

		}

		void MoveTowardsEntity(EntityID& id1, EntityID& id2)
		{
			if (EntityisOnTheRight(id1, id2))
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id1).Direction.x = 1;

			}
			else if(EntityisOnTheLeft(id1,id2))
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id1).Direction.x = -1;

			}

			if (EntityisOnTop(id1, id2))
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id1).Direction.y = 1;
			
			}
			else if(EntityisOnBot(id1, id2))
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id1).Direction.y = -1;
			}

		}

		void MoveTowardsPoint(EntityID& id1, Vector2& point)
		{
			Vector2 pos{ env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position };
			Vector2 diff{ point - pos };
			Vector2 temp = Normalise(diff);
			env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id1).Direction = temp;
		}

		bool EntityisAtPoint(EntityID& id1, Vector2& point)
		{
			Vector2 pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id1).position;
			return pos == point;
		}


	}


}