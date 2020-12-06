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
  public:
    ChaseEnemyLancer();
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class IdleFiddler : public AIState
  {
    Waypoint waypoint;
  public:
    IdleFiddler(Waypoint& wp, Vector2& charge_range);
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class ChaseEnemyFiddler : public AIState
  {
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
      float CooldownTimer;
      int CurrentPoint;
      SerpentipedeAIData SerpentData;

      ChaseEnemySerpentipede(SerpentipedeAIData& d);
      void onEnter(EntityID& id) override;
      void onExit(EntityID& id) override;
      void Update(EntityID& id1) override;
  };

}
