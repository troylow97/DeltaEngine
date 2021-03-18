/**********************************************************************************
* \file   AI_State.h
* \brief  The file contains the code for AI States
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <unordered_map>
#include "Waypoint.h"

namespace DeltaEngine
{
  class Transition;

  class AIState
  {
  protected:
    bool CheckEdges(EntityID&);
  public:
    std::unordered_map<std::string, Transition*> TransitionEdges;
    virtual void onEnter(EntityID& id) = 0;
    virtual void onExit(EntityID& id) = 0;
    virtual void Update(EntityID& id) = 0;
    virtual ~AIState();
  };

  struct SerpentipedeAIData
  {
    float AttackCooldown;
    Vector2 Points[3];
    Vector2 DetectionRange;
    SerpentipedeAIData();
    SerpentipedeAIData(SerpentipedeAIData& d);
  };

  struct FiddlerAIData
  {
    Waypoint waypoint;
    Vector2 ChargeDetectionRange;
    Vector2 LostDetectionRange;
    FiddlerAIData();
    FiddlerAIData(FiddlerAIData& d);
  };

  struct LancerAIData
  {
    Vector2 ChargeDetectionRange;
    LancerAIData();
    LancerAIData(LancerAIData& d);
  };

  class LancerSpawn : public AIState //Mosquito
  {
  public:
    LancerSpawn(Vector2& chase_range);
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class IdleLancer : public AIState //Mosquito
  {
  public:
    IdleLancer(Vector2& chase_range);
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class ChaseEnemyLancer : public AIState
  {
    bool Charging;
    bool Bouncing;
    bool Attacking;
    float BouncingTimer;
    float ChargeTimer;
    Vector2 Direction;
  public:
    ChaseEnemyLancer();
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class IdleFiddler : public AIState
  {
    float DurationBeforeExitState;
    Waypoint waypoint;
  public:
    IdleFiddler(Waypoint& wp, Vector2& charge_range);
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class ChaseEnemyFiddler : public AIState
  {
    float DurationBeforeExitState;
    float FacePlayerTimer;
    float AttackDelay;
    bool hasAttacked;
  public:
    ChaseEnemyFiddler(Vector2& lost_range);
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class IdleSerpentipede : public AIState
  {
  public:
    IdleSerpentipede(Vector2);
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class ChaseEnemySerpentipede : public AIState
  {
  public:
    bool Attacking;
    float CooldownTimer;
    float BurrowDownDuration;
    float BurrowUpDuration;
    float BurrowDownDelay;
    unsigned int BurrowState; //0: Seen, 1: Burrowing Down, 2: Hidden, 3: Burrowing Up
    int CurrentPoint;
    SerpentipedeAIData SerpentData;

    ChaseEnemySerpentipede(SerpentipedeAIData& d);
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };
}
