#include "EnemySpawner.h"
#include "ECS/ECSModule.h"
#include "Core/GlobalStruct.h"
#include "../../UnitManager.h"
#include "AI/AITools.h"
#include "../Source/Core/Utils/Random.h"
namespace DeltaEngine
{
	void EnemySpawner::SpawnEnemy(unsigned amount,std::string type, Vector2 position)
	{
		for (unsigned i = 0; i < amount; ++i)
		{
			float rand1 = Random::RandomFloatRange(-0.4, 0.4);

			EntityID enemy = env.pECS->GetWorld().GetEntityManager().CreateEntity();
			env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(enemy);
			env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(enemy);
			env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(enemy);
			env.pECS->GetWorld().GetEntityManager().AddComponent<Image>(enemy);
			env.pECS->GetWorld().GetEntityManager().AddComponent<State>(enemy);
			env.pECS->GetWorld().GetEntityManager().AddComponent<AI>(enemy);
			env.pECS->GetWorld().GetEntityManager().AddComponent<Attack>(enemy);
			env.pECS->GetWorld().GetEntityManager().AddComponent<Health>(enemy);
			env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(enemy);
			env.pECS->GetWorld().GetEntityManager().AddComponent<Renderer2D>(enemy);

			env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(enemy).position = position + rand1;
			env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(enemy).original_point = position + rand1;
			env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(enemy).type = EntityCategory::E_ENEMY;
			env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(enemy).CollisionLayerID = 4;
			env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(enemy).CollisionLayerCheck = 11;

			env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).MaxAcceleration = 0;

			if (type == "lancer")
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(enemy).key = "idle_lancer";
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).Movespeed = LancerData.Movespeed;
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).Mass = LancerData.Mass;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth = LancerData.Health;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).MaxHealth = LancerData.Health;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(enemy).MeleeDamage = LancerData.Damage;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(enemy).m_Sprite.m_Key = "Textures/Lancer";
				env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(enemy).m_Sprite.m_Index = 0;
			}
			else if (type == "fiddler")
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(enemy).key = "idle_fiddler";
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).hasGravity = true;
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).Movespeed = FiddlerData.Movespeed;
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).Mass = FiddlerData.Mass;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth = FiddlerData.Health;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).MaxHealth = FiddlerData.Health;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(enemy).MeleeDamage = FiddlerData.Damage;
			}
			else if (type == "serpentipede")
			{
				env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(enemy).key = "idle_serpentipede";
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).hasGravity = true;
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).Movespeed = SerpentipedeData.Movespeed;
				env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).Mass = SerpentipedeData.Mass;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth = SerpentipedeData.Health;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).MaxHealth = SerpentipedeData.Health;
				env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(enemy).MeleeDamage = SerpentipedeData.Damage;
			}

			EnemiesInGauntlet.push_back(enemy);
		}
	}

	Gauntlet::Gauntlet() :
		EnemyWaves{},
		ActivationPoint{{0,0}},
		CurrentEnemyWave{0},
		isActivated{false},
		isFinished{false}
	{}

	void EnemySpawner::LoadEnemyData()
	{
		//EnemyData Lancer;
		//Lancer.Health = 10;
		//Lancer.Damage = 1;
		//Lancer.Mass = 20;
		//Lancer.Movespeed = 30;
		
		//JsonFile file;
		//file.StartWriter("Enemy/Lancer.json").StartObject().WriteObject(Lancer).EndObject().EndWriter();
		//
		//JsonFile file2;
		//file2.StartWriter("Enemy/Fiddler.json").StartObject().WriteObject(Lancer).EndObject().EndWriter();
		//
		//JsonFile file3;
		//file3.StartWriter("Enemy/Serpentipede.json").StartObject().WriteObject(Lancer).EndObject().EndWriter();
		
		JsonFile file;
		file.StartReader("Enemy/Lancer.json").LoadObject(LancerData).EndReader();
		JsonFile file2;
		file.StartReader("Enemy/Fiddler.json").LoadObject(FiddlerData).EndReader();
		JsonFile file3;
		file.StartReader("Enemy/Serpentipede.json").LoadObject(SerpentipedeData).EndReader();
		
	}

	void EnemySpawner::Initialize()
	{
		LoadEnemyData();

		EnemyWave wave;
		wave.EnemyType = "lancer";
		wave.EnemyCount = 3;
		wave.SpawnArea = Vector2{ 6.0,0.0 };
		
		EnemyWave wave2;
		wave2.EnemyType = "serpentipede";
		wave2.EnemyCount = 2;
		wave2.SpawnArea = Vector2{ 5.0,0.0 };

		EnemyWave wave3;
		wave2.EnemyType = "fiddler";
		wave2.EnemyCount = 2;
		wave2.SpawnArea = Vector2{ 5.0,0.0 };

		std::vector<EnemyWave> firstwave;
		firstwave.push_back(wave);
		firstwave.push_back(wave2);

		std::vector<EnemyWave> secondwave;
		secondwave.push_back(wave3);

		std::vector<std::vector<EnemyWave>> Waves;
		Waves.push_back(firstwave);
		Waves.push_back(secondwave);
		
		Gauntlet g1;
		g1.ActivationPoint = Vector2{ 5.0,0.0 };
		g1.CurrentEnemyWave = 0;
		g1.EnemyWaves = Waves;
		
		EnemyWave wave4;
		wave4.SpawnArea = Vector2{ 9.0,0.0 };
		wave4.EnemyType = "fiddler";
		wave4.EnemyCount = 2;
		
		Gauntlet g2;
		g2.ActivationPoint = Vector2{ 20.0,0.0 };
		g2.CurrentEnemyWave = 0;
		g2.EnemyWaves.push_back({wave4});
		
		list.Gauntlets.push_back(g1);
		list.Gauntlets.push_back(g2);
		
		
		JsonFile file;
		file.StartWriter("EnemySpawns/GauntletPoints.json").StartObject().WriteObject(list).EndObject().EndWriter();
		//file.StartReader("EnemySpawns/GauntletPoints.json").LoadObject(list).EndReader();

		GauntletIsActive = false;
	}
	
	void EnemySpawner::Update()
	{
		//temporary add this line inside
		unit_manager.Initialize();
		EntityID player = GetUnitManager().player;

		for (int i = 0; i < list.Gauntlets.size(); ++i)
		{
			if (AITools::EntityisAtPoint(player,list.Gauntlets[i].ActivationPoint) && !list.Gauntlets[i].isActivated && !list.Gauntlets[i].isFinished)
			{
				CurrentGauntlet = i;
				GauntletIsActive = true;
				list.Gauntlets[i].isActivated = true;
				//lock player
				break;
			}
		}

		if (GauntletIsActive)
		{
			for (std::vector<EntityID>::iterator it = EnemiesInGauntlet.begin();it != EnemiesInGauntlet.end();)
			{
				if (env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(*it).CurrentHealth <= 0)
				{
					it = EnemiesInGauntlet.erase(it);
				}
				else
					++it;
			}

			if (EnemiesInGauntlet.empty())
			{
				const auto CurrentWave = list.Gauntlets[CurrentGauntlet].CurrentEnemyWave;
				if (CurrentWave < list.Gauntlets[CurrentGauntlet].EnemyWaves.size())
				{
					for(size_t i = 0; i < list.Gauntlets[CurrentGauntlet].EnemyWaves[CurrentWave].size(); ++i)
					{
						EnemyWave wave = list.Gauntlets[CurrentGauntlet].EnemyWaves[CurrentWave][i];
						SpawnEnemy(wave.EnemyCount, wave.EnemyType, wave.SpawnArea);
						list.Gauntlets[CurrentGauntlet].CurrentEnemyWave++;
					}
				}
				else
				{
					list.Gauntlets[CurrentGauntlet].isActivated = false;
					list.Gauntlets[CurrentGauntlet].isFinished = true;
					GauntletIsActive = false;
				}
			}

		}




	}
	
	void EnemySpawner::LateUpdate()
	{
	
	}
}