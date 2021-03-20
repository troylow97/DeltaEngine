/**********************************************************************************
* \file   EnemySpawner.cpp
* \brief  The file contains the system for spawning enemies in the game
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
    EnemyWaves{},
    ActivationPoint{{0, 0}},
    CurrentEnemyWave{0},
    isActivated{false},
    isFinished{false}
  {
  }

  void EnemySpawner::LoadEnemyData()
  {
    JsonFile file;
    file.StartReader("Enemy/Lancer.json").LoadObject(LancerData).EndReader();
    file.StartReader("Enemy/Fiddler.json").LoadObject(FiddlerData).EndReader();
    file.StartReader("Enemy/Serpentipede.json").LoadObject(SerpentipedeData).EndReader();
  }

  void EnemySpawner::Initialize()
  {
    LoadEnemyData();

    JsonFile file;
    file.StartReader("EnemySpawns/GauntletPoints.json").LoadObject(list).EndReader();
    for (size_t i = 0; i < list.Gauntlets.size(); ++i)
      file.StartReader("EnemySpawns/GauntletPoints.json").LoadObject(list.Gauntlets[i]).EndReader();

    GauntletIsActive = false;

    ResetActivationPointBool();
  }

  void EnemySpawner::Update()
  {
    if (!ActivateGauntlet)
        return;
  	
    if (env.pECS->GetWorld().GetEntityManager().IsEntityValid(UnitManager::GetPlayerID()))
      if (env.pECS->GetWorld().GetEntityManager().HasComponent<Player>(UnitManager::GetPlayerID()))
      {
        EntityID player = UnitManager::GetPlayerID();

        for (auto i = 0; i < list.Gauntlets.size(); ++i)
        {
          auto& Gauntlet = list.Gauntlets[i];
          if (AITools::EntityisAtPointInX(player, list.Gauntlets[i].ActivationPoint.x, 0.3f) && !Gauntlet.isActivated &&
            !Gauntlet.isFinished)
          {
            ActivationPoint = list.Gauntlets[i].ActivationPoint;
            CurrentGauntlet = i;
            GauntletIsActive = true;
            Gauntlet.isActivated = true;
            break;
          }
        }

        if (GauntletIsActive)
        {
          if (CheckForOutsideEnemies())
            return;

          for (auto it = SpawnedEnemiesInGauntlet.begin(); it != SpawnedEnemiesInGauntlet.end();)
          {
            if (!env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(*it))
            {
              it = SpawnedEnemiesInGauntlet.erase(it);
              continue;
            }

            if (env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(*it) && env.pECS->GetWorld().
              GetEntityManager().GetComponent<Health>(*it).CurrentHealth <= 0)
            {
              it = SpawnedEnemiesInGauntlet.erase(it);
            }
            else
              ++it;
          }

          if (SpawnedEnemiesInGauntlet.empty())
          {
            auto& Gauntlet = list.Gauntlets[CurrentGauntlet];
            auto& CurrentWave = Gauntlet.CurrentEnemyWave;
            //lock player camera

            if (CurrentWave < list.Gauntlets[CurrentGauntlet].EnemyWaves.size())
            {
              if (CurrentWave == 0)
              {
                //spawn walls
                GauntletWalls[0] = SpawnWall(Vector2{
                  Gauntlet.ActivationPoint.x + Gauntlet.WallOffsetRight, Gauntlet.ActivationPoint.y
                });
                GauntletWalls[1] = SpawnWall(Vector2{
                  Gauntlet.ActivationPoint.x - Gauntlet.WallOffsetRight, Gauntlet.ActivationPoint.y
                });
              }
              for (size_t i = 0; i < list.Gauntlets[CurrentGauntlet].EnemyWaves[CurrentWave].size(); ++i)
              {
                const EnemyWave wave = list.Gauntlets[CurrentGauntlet].EnemyWaves[CurrentWave][i];
                SpawnEnemy(wave.EnemyCount, wave.EnemyType, wave.SpawnArea);
              }
              CurrentWave++;
            }
            else
            {
              //em.DestroyEntity(GauntletWalls[0]);
              //em.DestroyEntity(GauntletWalls[1]);
              env.pECS->GetWorld().GetEntityManager().AddComponent<Lifespan>(GauntletWalls[0]);
              env.pECS->GetWorld().GetEntityManager().GetComponent<Lifespan>(GauntletWalls[0]).Timer = 1.0f;
              env.pECS->GetWorld().GetEntityManager().AddComponent<Lifespan>(GauntletWalls[1]);
              env.pECS->GetWorld().GetEntityManager().GetComponent<Lifespan>(GauntletWalls[1]).Timer = 1.0f;
              em.GetComponent<State>(GauntletWalls[0]).SetBool("BlockerUp", false);
              em.GetComponent<State>(GauntletWalls[0]).SetBool("BlockerDown", true);
              em.GetComponent<State>(GauntletWalls[1]).SetBool("BlockerUp", false);
              em.GetComponent<State>(GauntletWalls[1]).SetBool("BlockerDown", true);
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
    for (int i = 0; i < list.Gauntlets.size(); ++i)
    {
      list.Gauntlets[i].isActivated = false;
      list.Gauntlets[i].isFinished = false;
    }
  }

  bool EnemySpawner::CheckForOutsideEnemies()
  {
    bool activated = false;
    em.ForEach([&](EntityID& id, EntityType et)
    {
      if (et.type == EntityCategory::E_ENEMY && AITools::EntityisAtPoint(id, ActivationPoint, 3.0f))
      {
        activated = true;
      }
    });
    return activated;
  }

  void EnemySpawner::Shutdown()
  {
    EnemiesInGauntlet.clear();
    SpawnedEnemiesInGauntlet.clear();
    GauntletIsActive = false;
    ResetActivationPointBool();
  }

  EntityID EnemySpawner::SpawnWall(Vector2 position)
  {
    EntityID wall = env.pECS->GetWorld().GetEntityManager().CreateEntity();
    em.AddComponent<RigidBody>(wall);
    em.AddComponent<Collider>(wall);
    em.AddComponent<Image>(wall);
    em.AddComponent<Renderer2D>(wall);
    em.AddComponent<Animator>(wall);
    em.AddComponent<State>(wall);
  	
    em.GetComponent<EntityType>(wall).type = EntityCategory::E_WALL;
    em.GetComponent<Transform>(wall).position = position;
    em.GetComponent<Transform>(wall).position.y += 1.8f;
    em.GetComponent<Transform>(wall).scale = Vector2{1.0, 1.0};
    em.GetComponent<RigidBody>(wall).isMoveable = false;
    em.GetComponent<Collider>(wall).CollisionLayerID = 1;
    em.GetComponent<Collider>(wall).CollisionLayerCheck = 11;
    em.GetComponent<Collider>(wall).offset.y = -1;
    em.GetComponent<Collider>(wall).size = { 2,3 };
    em.GetComponent<Renderer2D>(wall).m_Wireframe = false;
    em.GetComponent<Renderer2D>(wall).m_SortingLayer = 2;
    em.GetComponent<Image>(wall).m_Sprite.m_Key = "Textures/BLOCKER_SPAWN";
    em.GetComponent<Animator>(wall).m_ControllerKey = "Animation/Blocker";
    em.GetComponent<State>(wall).SetBool("BlockerUp", true);
    return wall;
  }

  void EnemySpawner::SpawnEnemy(unsigned amount, std::string type, Vector2 position)
  {
    for (unsigned i = 0; i < amount; ++i)
    {
      float rand1 = Random::RandomFloatRange(-0.4f, 0.4f);

      EntityID enemy = env.pECS->GetWorld().GetEntityManager().CreateEntity();
      em.AddComponent<RigidBody>(enemy);
      em.AddComponent<Collider>(enemy);
      em.AddComponent<Image>(enemy);
      em.AddComponent<State>(enemy);
      em.AddComponent<AI>(enemy);
      em.AddComponent<Attack>(enemy);
      em.AddComponent<Health>(enemy);
      em.AddComponent<Renderer2D>(enemy);
      em.AddComponent<Animator>(enemy);

      em.GetComponent<Renderer2D>(enemy).m_Wireframe = false;
      em.GetComponent<Transform>(enemy).position = position + rand1;
      em.GetComponent<AI>(enemy).original_point = position + rand1;
      em.GetComponent<EntityType>(enemy).type = EntityCategory::E_ENEMY;
      em.GetComponent<Collider>(enemy).CollisionLayerID = 4;
      em.GetComponent<Collider>(enemy).CollisionLayerCheck = 9;
      //env.pECS->GetWorld().GetEntityManager().GetComponent<Animator>(enemy).m_ControllerKey = "Animation/Dave";
      env.pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(enemy).m_SortingLayer = 4;

      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).MaxAcceleration = 0;

      if (type == "lancer")
      {
        em.GetComponent<AI>(enemy).key = "lancer_spawn";
        em.GetComponent<Collider>(enemy).CollisionLayerCheck = 8;
        em.GetComponent<RigidBody>(enemy).Movespeed = LancerData.Movespeed;
        em.GetComponent<RigidBody>(enemy).Mass = LancerData.Mass;
        em.GetComponent<Health>(enemy).CurrentHealth = LancerData.Health;
        em.GetComponent<Health>(enemy).MaxHealth = LancerData.Health;
        em.GetComponent<Attack>(enemy).MeleeDamage = static_cast<int>(LancerData.Damage);
        em.GetComponent<Transform>(enemy).scale = LancerData.TransformScale;
        em.GetComponent<Collider>(enemy).offset = LancerData.ColliderOffset;
        em.GetComponent<Collider>(enemy).size = LancerData.ColliderScale;
        em.GetComponent<Image>(enemy).m_Sprite.m_Key = "Textures/Lancer";
        em.GetComponent<Animator>(enemy).m_ControllerKey = "Animation/Lancer";
        em.GetComponent<Image>(enemy).m_Sprite.m_Index = 0;
      }
      else if (type == "fiddler")
      {
        em.GetComponent<AI>(enemy).key = "idle_fiddler";
        em.GetComponent<RigidBody>(enemy).hasGravity = true;
        em.GetComponent<RigidBody>(enemy).Movespeed = FiddlerData.Movespeed;
        em.GetComponent<RigidBody>(enemy).Mass = FiddlerData.Mass;
        em.GetComponent<Health>(enemy).CurrentHealth = FiddlerData.Health;
        em.GetComponent<Health>(enemy).MaxHealth = FiddlerData.Health;
        em.GetComponent<Attack>(enemy).MeleeDamage = static_cast<int>(FiddlerData.Damage);
        em.GetComponent<Transform>(enemy).scale = FiddlerData.TransformScale;
        em.GetComponent<Collider>(enemy).offset = FiddlerData.ColliderOffset;
        em.GetComponent<Collider>(enemy).size = FiddlerData.ColliderScale;
        em.GetComponent<Image>(enemy).m_Sprite.m_Key = "Textures/FID_WALK";
        em.GetComponent<Animator>(enemy).m_ControllerKey = "Animation/Fiddler";
        em.GetComponent<Image>(enemy).m_Sprite.m_Index = 0;
      }
      else if (type == "serpentipede")
      {
        em.GetComponent<AI>(enemy).key = "idle_serpentipede";
        em.GetComponent<RigidBody>(enemy).hasGravity = true;
        em.GetComponent<RigidBody>(enemy).Movespeed = SerpentipedeData.Movespeed;
        em.GetComponent<RigidBody>(enemy).Mass = SerpentipedeData.Mass;
        em.GetComponent<Health>(enemy).CurrentHealth = SerpentipedeData.Health;
        em.GetComponent<Health>(enemy).MaxHealth = SerpentipedeData.Health;
        em.GetComponent<Attack>(enemy).MeleeDamage = static_cast<int>(SerpentipedeData.Damage);
        em.GetComponent<Transform>(enemy).scale = SerpentipedeData.TransformScale;
        em.GetComponent<Collider>(enemy).offset = SerpentipedeData.ColliderOffset;
        em.GetComponent<Collider>(enemy).size = SerpentipedeData.ColliderScale;
        em.GetComponent<Image>(enemy).m_Sprite.m_Key = "Textures/SERP_FULL_IDLE";
        em.GetComponent<Animator>(enemy).m_ControllerKey = "Animation/Serpentipede";
        em.GetComponent<Image>(enemy).m_Sprite.m_Index = 0;

        unsigned rand_sound = Random::RandomIntRange(0, 2);
        switch (rand_sound)
        {
        case 0:
          AudioEngine::Play("Audio/Serpentipede/Burrow1.ogg");
          break;
        case 1:
          AudioEngine::Play("Audio/Serpentipede/Burrow2.ogg");
          break;
        }
      }

      SpawnedEnemiesInGauntlet.push_back(enemy);
    }
  }
}
