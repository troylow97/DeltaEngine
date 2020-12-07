#include "EnemySpawner.h"
#include "ECS/ECSModule.h"
#include "Core/GlobalStruct.h"
#include "../UnitManager.h"
#include "../AI/AITools.h"
#include "../Source/Core/Utils/Random.h"
#include "Audio/AudioEngine.h"

namespace DeltaEngine
{
Gauntlet::Gauntlet() :
  EnemyWaves {},
  ActivationPoint { {0,0} },
  CurrentEnemyWave { 0 },
  isActivated { false },
  isFinished { false }
{}

void EnemySpawner::LoadEnemyData()
{
  //EnemyData Lancer;
  //Lancer.TransformScale = { 1.0,1.0 };
  //Lancer.ColliderScale = { 1.0,1.0 };
  //Lancer.ColliderOffset = { 0,0 };
  //Lancer.Health = 10;
  //Lancer.Damage = 1;
  //Lancer.Mass = 20;
  //Lancer.Movespeed = 30;
  //
  //JsonFile file;
  //file.StartWriter("Enemy/Lancer.json").StartObject().WriteObject(Lancer).EndObject().EndWriter();
  //
  //JsonFile file2;
  //file2.StartWriter("Enemy/Fiddler.json").StartObject().WriteObject(Lancer).EndObject().EndWriter();
  //
  //JsonFile file3;
  //file3.StartWriter("Enemy/Serpentipede.json").StartObject().WriteObject(Lancer).EndObject().EndWriter();

  JsonFile file;
  file.StartReader( "Enemy/Lancer.json" ).LoadObject( LancerData ).EndReader();
  JsonFile file2;
  file.StartReader( "Enemy/Fiddler.json" ).LoadObject( FiddlerData ).EndReader();
  JsonFile file3;
  file.StartReader( "Enemy/Serpentipede.json" ).LoadObject( SerpentipedeData ).EndReader();

}

void EnemySpawner::Initialize()
{
  LoadEnemyData();

  //EnemyWave wave;
  //wave.EnemyType = "lancer";
  //wave.EnemyCount = 3;
  //wave.SpawnArea = Vector2{ 6.0,0.0 };
  //
  //EnemyWave wave2;
  //wave2.EnemyType = "serpentipede";
  //wave2.EnemyCount = 2;
  //wave2.SpawnArea = Vector2{ 5.0,0.0 };
  //
  //EnemyWave wave3;
  //wave2.EnemyType = "fiddler";
  //wave2.EnemyCount = 2;
  //wave2.SpawnArea = Vector2{ 5.0,0.0 };
  //
  //std::vector<EnemyWave> firstwave;
  //firstwave.push_back(wave);
  //firstwave.push_back(wave2);
  //
  //std::vector<EnemyWave> secondwave;
  //secondwave.push_back(wave3);
  //
  //std::vector<std::vector<EnemyWave>> Waves;
  //Waves.push_back(firstwave);
  //Waves.push_back(secondwave);
  //
  //Gauntlet g1;
  //g1.ActivationPoint = Vector2{ 5.0,0.0 };
  //g1.CurrentEnemyWave = 0;
  //g1.EnemyWaves = Waves;
  //g1.WallOffsetRight = 7.0f;
  //g1.WallOffsetLeft = 7.0f;
  //
  //EnemyWave wave4;
  //wave4.SpawnArea = Vector2{ 9.0,0.0 };
  //wave4.EnemyType = "fiddler";
  //wave4.EnemyCount = 2;
  //
  //Gauntlet g2;
  //g2.ActivationPoint = Vector2{ 20.0,0.0 };
  //g2.CurrentEnemyWave = 0;
  //g2.EnemyWaves.push_back({wave4});
  //g2.WallOffsetRight = 7.0f;
  //g2.WallOffsetLeft = 7.0f;

  //list.Gauntlets.push_back(g1);
  //list.Gauntlets.push_back(g2);
  JsonFile file;
  //file.StartWriter("EnemySpawns/GauntletPoints.json").StartObject().WriteObject(list).EndObject().EndWriter();
  file.StartReader( "EnemySpawns/GauntletPoints.json" ).LoadObject( list ).EndReader();
  for ( size_t i = 0; i < list.Gauntlets.size(); ++i )
    file.StartReader( "EnemySpawns/GauntletPoints.json" ).LoadObject( list.Gauntlets[i] ).EndReader();

  GauntletIsActive = false;

  ResetActivationPointBool();
}

void EnemySpawner::Update()
{
  if ( env.pECS->GetWorld().GetEntityManager().IsEntityValid( UnitManager::GetPlayerID() ) )
    if ( env.pECS->GetWorld().GetEntityManager().HasComponent<Player>( UnitManager::GetPlayerID() ) )
    {
      EntityID player = UnitManager::GetPlayerID();

      for ( int i = 0; i < list.Gauntlets.size(); ++i )
      {
        auto &Gauntlet = list.Gauntlets[i];
        if ( AITools::EntityisAtPointInX( player, list.Gauntlets[i].ActivationPoint.x, 0.3f ) && !Gauntlet.isActivated && !Gauntlet.isFinished )
        {
          ActivationPoint = list.Gauntlets[i].ActivationPoint;
          CurrentGauntlet = i;
          GauntletIsActive = true;
          Gauntlet.isActivated = true;
          break;
        }
      }

      if ( GauntletIsActive )
      {
        if ( CheckForOutsideEnemies() )
          return;

        for ( auto it = SpawnedEnemiesInGauntlet.begin(); it != SpawnedEnemiesInGauntlet.end();)
        {
          if ( !env.pECS->GetWorld().GetEntityManager().HasComponent<Health>( *it ) )
          {
            it = SpawnedEnemiesInGauntlet.erase( it );
            continue;
          }

          if ( env.pECS->GetWorld().GetEntityManager().GetComponent<Health>( *it ).CurrentHealth <= 0 )
          {
            it = SpawnedEnemiesInGauntlet.erase( it );
          }
          else
            ++it;
        }

        if ( SpawnedEnemiesInGauntlet.empty() )
        {
          auto &Gauntlet = list.Gauntlets[CurrentGauntlet];
          auto &CurrentWave = Gauntlet.CurrentEnemyWave;
          //lock player camera

          if ( CurrentWave < list.Gauntlets[CurrentGauntlet].EnemyWaves.size() )
          {
            if ( CurrentWave == 0 )
            {
              //spawn walls
              GauntletWalls[0] = SpawnWall( Vector2 { Gauntlet.ActivationPoint.x + Gauntlet.WallOffsetRight,Gauntlet.ActivationPoint.y } );
              GauntletWalls[1] = SpawnWall( Vector2 { Gauntlet.ActivationPoint.x - Gauntlet.WallOffsetRight,Gauntlet.ActivationPoint.y } );
            }
            for ( size_t i = 0; i < list.Gauntlets[CurrentGauntlet].EnemyWaves[CurrentWave].size(); ++i )
            {
              const EnemyWave wave = list.Gauntlets[CurrentGauntlet].EnemyWaves[CurrentWave][i];
              SpawnEnemy( wave.EnemyCount, wave.EnemyType, wave.SpawnArea );
            }
            CurrentWave++;
          }
          else
          {
            em.DestroyEntity( GauntletWalls[0] );
            em.DestroyEntity( GauntletWalls[1] );
            list.Gauntlets[CurrentGauntlet].isActivated = false;
            list.Gauntlets[CurrentGauntlet].isFinished = true;
            GauntletIsActive = false;
          }
        }

      }
    }
}

void EnemySpawner::LateUpdate()
{

}

void EnemySpawner::ResetActivationPointBool()
{
  for ( int i = 0; i < list.Gauntlets.size(); ++i )
  {
    list.Gauntlets[i].isActivated = false;
    list.Gauntlets[i].isFinished = false;
  }
}

bool EnemySpawner::CheckForOutsideEnemies()
{
  bool activated = false;
  em.ForEach( [&]( EntityID &id, EntityType et )
  {
    if ( et.type == EntityCategory::E_ENEMY && AITools::EntityisAtPoint( id, ActivationPoint, 3.0f ) )
    {
      activated = true;
    }
  } );
  return activated;
}

void EnemySpawner::Shutdown()
{
  EnemiesInGauntlet.clear();
  SpawnedEnemiesInGauntlet.clear();
  GauntletIsActive = false;
  ResetActivationPointBool();
}

EntityID EnemySpawner::SpawnWall( Vector2 position )
{
  EntityID wall = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>( wall );
  env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>( wall );
  //env.pECS->GetWorld().GetEntityManager().AddComponent<Image>(wall);
  //env.pECS->GetWorld().GetEntityManager().AddComponent<Renderer2D>(wall);
  env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>( wall ).type = EntityCategory::E_WALL;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>( wall ).position = position;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>( wall ).scale = Vector2 { 1.0,6.0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( wall ).isMoveable = false;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( wall ).CollisionLayerID = 1;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( wall ).CollisionLayerCheck = 14;
  return wall;
}

void EnemySpawner::SpawnEnemy( unsigned amount, std::string type, Vector2 position )
{
  for ( unsigned i = 0; i < amount; ++i )
  {
    float rand1 = Random::RandomFloatRange( -0.4f, 0.4f );

    EntityID enemy = env.pECS->GetWorld().GetEntityManager().CreateEntity();
    env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>( enemy );
    env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>( enemy );
    env.pECS->GetWorld().GetEntityManager().AddComponent<Image>( enemy );
    env.pECS->GetWorld().GetEntityManager().AddComponent<State>( enemy );
    env.pECS->GetWorld().GetEntityManager().AddComponent<AI>( enemy );
    env.pECS->GetWorld().GetEntityManager().AddComponent<Attack>( enemy );
    env.pECS->GetWorld().GetEntityManager().AddComponent<Health>( enemy );
    env.pECS->GetWorld().GetEntityManager().AddComponent<Renderer2D>( enemy );
    env.pECS->GetWorld().GetEntityManager().AddComponent<Animator>( enemy );

    env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>( enemy ).position = position + rand1;
    env.pECS->GetWorld().GetEntityManager().GetComponent<AI>( enemy ).original_point = position + rand1;
    env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>( enemy ).type = EntityCategory::E_ENEMY;
    env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( enemy ).CollisionLayerID = 4;
    env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( enemy ).CollisionLayerCheck = 9;
    //env.pECS->GetWorld().GetEntityManager().GetComponent<Animator>(enemy).m_ControllerKey = "Animation/Dave";
    env.pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>( enemy ).m_SortingLayer = 4;

    env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( enemy ).MaxAcceleration = 0;

    if ( type == "lancer" )
    {
      env.pECS->GetWorld().GetEntityManager().GetComponent<AI>( enemy ).key = "lancer_spawn";
      env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( enemy ).CollisionLayerCheck = 1;
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( enemy ).Movespeed = LancerData.Movespeed;
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( enemy ).Mass = LancerData.Mass;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Health>( enemy ).CurrentHealth = LancerData.Health;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Health>( enemy ).MaxHealth = LancerData.Health;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>( enemy ).MeleeDamage = (int) LancerData.Damage;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>( enemy ).scale = LancerData.TransformScale;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( enemy ).offset = LancerData.ColliderOffset;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( enemy ).size = LancerData.ColliderScale;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Image>( enemy ).m_Sprite.m_Key = "Textures/Lancer";
      env.pECS->GetWorld().GetEntityManager().GetComponent<Image>( enemy ).m_Sprite.m_Index = 0;
    }
    else if ( type == "fiddler" )
    {
      env.pECS->GetWorld().GetEntityManager().GetComponent<AI>( enemy ).key = "idle_fiddler";
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( enemy ).hasGravity = true;
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( enemy ).Movespeed = FiddlerData.Movespeed;
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( enemy ).Mass = FiddlerData.Mass;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Health>( enemy ).CurrentHealth = FiddlerData.Health;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Health>( enemy ).MaxHealth = FiddlerData.Health;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>( enemy ).MeleeDamage = (int) FiddlerData.Damage;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>( enemy ).scale = FiddlerData.TransformScale;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( enemy ).offset = FiddlerData.ColliderOffset;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( enemy ).size = FiddlerData.ColliderScale;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Image>( enemy ).m_Sprite.m_Key = "Textures/FID_WALK";
      env.pECS->GetWorld().GetEntityManager().GetComponent<Image>( enemy ).m_Sprite.m_Index = 0;
    }
    else if ( type == "serpentipede" )
    {
      env.pECS->GetWorld().GetEntityManager().GetComponent<AI>( enemy ).key = "idle_serpentipede";
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( enemy ).hasGravity = true;
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( enemy ).Movespeed = SerpentipedeData.Movespeed;
      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>( enemy ).Mass = SerpentipedeData.Mass;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Health>( enemy ).CurrentHealth = SerpentipedeData.Health;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Health>( enemy ).MaxHealth = SerpentipedeData.Health;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>( enemy ).MeleeDamage = (int) SerpentipedeData.Damage;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>( enemy ).scale = SerpentipedeData.TransformScale;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( enemy ).offset = SerpentipedeData.ColliderOffset;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>( enemy ).size = SerpentipedeData.ColliderScale;
      env.pECS->GetWorld().GetEntityManager().GetComponent<Image>( enemy ).m_Sprite.m_Key = "Textures/SERP_FULL_IDLE";
      env.pECS->GetWorld().GetEntityManager().GetComponent<Image>( enemy ).m_Sprite.m_Index = 0;

      unsigned rand_sound = Random::RandomIntRange( 0, 2 );
      switch ( rand_sound )
      {
        case 0:
          AudioEngine::Play( "Audio/Serpentipede/Burrow1.ogg" );
          break;
        case 1:
          AudioEngine::Play( "Audio/Serpentipede/Burrow2.ogg" );
          break;
      }
    }

    SpawnedEnemiesInGauntlet.push_back( enemy );
  }
}
}
