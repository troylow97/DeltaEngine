#include "AI_State.h"
#include "AI_Transition.h"
#include "Core/Utils/Random.h"
#include "Core/GlobalStruct.h"
namespace DeltaEngine
{
	bool AIState::CheckEdges(EntityID& monster)
	{
		for (auto& it : TransitionEdges)
		{
			if (it.second->TestEdge(monster))
				return true;
		}

		return false;
	}
	AIState::~AIState()
	{
		for (auto& it : TransitionEdges)
		{
			delete it.second;
		}
	}
	IdleLancer::IdleLancer()
	{
		TransitionEdges["detect_enemy_lancer"] = new DetectEnemyLancer();
	}
	void IdleLancer::onEnter(EntityID& id)
	{

	}
	void IdleLancer::onExit(EntityID& id)
	{

	}
	void IdleLancer::Update(EntityID& monster)
	{
		CheckEdges(monster);
	}
//----------------------------------------------------------------------
	ChaseEnemyLancer::ChaseEnemyLancer()
	{
		TransitionEdges["lost_enemy_lancer"] = new LostEnemyLancer();
	}
	void ChaseEnemyLancer::onEnter(EntityID& id)
	{

	}
	void ChaseEnemyLancer::onExit(EntityID& id)
	{
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = { 0,0 };
	}
	void ChaseEnemyLancer::Update(EntityID& monster)
	{

		CheckEdges(monster);
		env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
		{
			AITools::MoveTowardsEntity(monster, player);
		});
	}
//----------------------------------------------------------------------

	IdleFiddler::IdleFiddler(Vector2 p1,Vector2 p2) :
		CurrentWayPoint(0)
	{
		WayPoints[0] = p1;
		WayPoints[1] = p2;
		TransitionEdges["detect_enemy_fiddler"] = new DetectEnemyFiddler();
	}
	void IdleFiddler::onEnter(EntityID& id)
	{

	}
	void IdleFiddler::onExit(EntityID& id)
	{

	}
	void IdleFiddler::Update(EntityID& monster)
	{
		//if (CurrentWayPoint == 0)
		{
			AITools::MoveTowardsPoint(monster, WayPoints[1]);
		}



		CheckEdges(monster);
	}
	ChaseEnemyFiddler::ChaseEnemyFiddler()
	{
		TransitionEdges["lost_enemy_fiddler"] = new LostEnemyFiddler();
	}
	void ChaseEnemyFiddler::onEnter(EntityID& id)
	{

	}
	void ChaseEnemyFiddler::onExit(EntityID& id)
	{
		env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = { 0,0 };
	}
	void ChaseEnemyFiddler::Update(EntityID& monster)
	{

		CheckEdges(monster);
		env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
			{
				AITools::MoveTowardsEntity(monster, player);
			});
	}
	//----------------------------------------------------------------------

}