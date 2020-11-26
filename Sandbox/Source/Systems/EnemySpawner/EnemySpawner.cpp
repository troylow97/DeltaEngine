#include "EnemySpawner.h"
#include "ECS/ECSModule.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
	void SpawnEnemy(std::string type, Vector2 position)
	{
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

		env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(enemy).position = position;
		env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(enemy).original_point = position;
		env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(enemy).type == EntityCategory::E_ENEMY;

		if (type == "lancer")
		{

			env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(enemy).key = "idle_lancer";
		}
		else if (type == "fiddler")
		{
			env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(enemy).key = "idle_fiddler";
		}
		else if (type == "serpentipede")
		{
			env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(enemy).key = "idle_serpentipede";
		}
	}

	Gauntlet::Gauntlet() :
		EnemyWaves{},
		ActivationPoint{{0,0}},
		isActivated{false}
	{}

	void EnemySpawner::Initialize()
	{
		//EnemyWave wave;
		//wave.EnemyType = "lancer";
		//wave.EnemyCount = 3;
		//wave.SpawnArea = Vector2{ 6.0,0.0 };
		//
		//Gauntlet g1;
		//g1.ActivationPoint = Vector2{ 5.0,0.0 };
		//g1.EnemyWaves.push_back(wave);
		//
		//EnemyWave wave2;
		//wave2.SpawnArea = Vector2{ 9.0,0.0 };
		//wave2.EnemyType = "fiddler";
		//wave2.EnemyCount = 2;
		//
		//Gauntlet g2;
		//g2.ActivationPoint = Vector2{ 8.0,0.0 };
		//g2.EnemyWaves.push_back(wave2);
		//
		//list.Gauntlets.push_back(g1);
		//list.Gauntlets.push_back(g2);
		//
		//
		JsonFile file;
		//file.StartWriter("EnemySpawns/GauntletPoints.json").StartObject().WriteObject(list).EndObject().EndWriter();
		file.StartReader("EnemySpawns/GauntletPoints.json").LoadObject(list).EndReader();
	}
	
	void EnemySpawner::Update()
	{
		
	}
	
	void EnemySpawner::LateUpdate()
	{
	
	}
}