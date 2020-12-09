/**********************************************************************************
* \file   EnemySpawner.h
* \brief  The file contains the system for spawning enemies in the game
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
    int EnemyCount{0};
    std::string EnemyType;
    Vector2 SpawnArea;
  };

  using VectorOfEnemyWaves = std::vector<std::vector<EnemyWave>>;

  struct Gauntlet
  {
    VectorOfEnemyWaves EnemyWaves;
    Vector2 ActivationPoint;
    float WallOffsetRight;
    float WallOffsetLeft;
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
    void EnemySpawner::Shutdown() override;
    void EnemySpawner::Update() override;
    void EnemySpawner::LateUpdate() override;
    GauntletsList list;
    void EnemySpawner::ResetActivationPointBool();

  private:
    void LoadEnemyData();
    bool CheckForOutsideEnemies();
    EntityID SpawnWall(Vector2 position);
    void SpawnEnemy(unsigned amount, std::string type, Vector2 position);

    std::vector<EntityID> SpawnedEnemiesInGauntlet;
    std::vector<EntityID> EnemiesInGauntlet;
    EntityID GauntletWalls[2];
    EnemyData SerpentipedeData;
    EnemyData FiddlerData;
    EnemyData LancerData;
    Vector2 ActivationPoint;
    int CurrentGauntlet{0};
    bool GauntletIsActive;

  END_DEFINE_SYSTEM(EnemySpawner)
}
