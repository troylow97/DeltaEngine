#pragma once
#include "Components/EntityType.h"
#include "Core/Math/Vector.h"
#include <string>
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
	void SpawnEnemy(std::string type, Vector2 position);

	struct EnemyWave
	{
		int EnemyCount;
		std::string EnemyType;
		Vector2 SpawnArea;
	};

	struct Gauntlet
	{
		std::vector<EnemyWave> EnemyWaves;
		Vector2 ActivationPoint;
		bool isActivated;

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
		GauntletsList list;
	    int WavesToSpawn;
	    int CurrentWave;
	
	    END_DEFINE_SYSTEM(AttackSystem)


}
