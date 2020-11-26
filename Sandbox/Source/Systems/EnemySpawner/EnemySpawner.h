#pragma once
#include "Components/EntityType.h"
#include "Core/Math/Vector.h"
#include <string>
#include "ECS/ECSModule.h"
#include "EnemyData.h"

namespace DeltaEngine
{

	
	struct EnemyWave
	{
		int EnemyCount;
		std::string EnemyType;
		Vector2 SpawnArea;
	};

	using VectorOfEnemyWaves = std::vector<std::vector<EnemyWave>>;
	
	struct Gauntlet
	{
		VectorOfEnemyWaves EnemyWaves;
		Vector2 ActivationPoint;
		unsigned CurrentEnemyWave;
		bool isActivated;
		bool isFinished;

		Gauntlet();
	};

	struct GauntletsList
	{
		std::vector<Gauntlet> Gauntlets;
	};

	DEFINE_SYSTEM(EnemySpawner, Transform, EntityType)
		void EnemySpawner::Initialize() override;
		void EnemySpawner::Update() override;
		void EnemySpawner::LateUpdate() override;

	
	
	private:
		void LoadEnemyData();
		void SpawnEnemy(unsigned amount, std::string type, Vector2 position);
		std::vector<EntityID> EnemiesInGauntlet;
		GauntletsList list;
		EnemyData SerpentipedeData;
		EnemyData FiddlerData;
		EnemyData LancerData;
		int CurrentGauntlet;
		bool GauntletIsActive;
	
	    END_DEFINE_SYSTEM(AttackSystem)


}
