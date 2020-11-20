/**********************************************************************************
* \file			AIStates
* \brief		Contains all the base abstract class of any AI State
* \author		Low Yee Troy, 100% Code Contribution
* \version		1.0
* \date			2020
*
* \note			Course: GAM200
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include <unordered_map>
#include "Core/GlobalStruct.h"
#include "AI/AITools.h"
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
  
  class IdleLancer : public AIState //Mosquito
  {
  public:
    IdleLancer();
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

  class IdleFiddler : public AIState, public Waypoint
  {
     
  public:

    IdleFiddler(Vector2 p1, Vector2 p2);
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class ChaseEnemyFiddler : public AIState
  {
  public:
    ChaseEnemyFiddler();
    void onEnter(EntityID& id) override;
    void onExit(EntityID& id) override;
    void Update(EntityID& id1) override;
  };

  class IdleSerpentipede : public AIState
  {
      unsigned int CurrentWayPoint;
  public:
      std::vector<Vector2> WayPoints;
      IdleSerpentipede(Vector2 p1, Vector2 p2,Vector2 p3);
      void onEnter(EntityID& id) override;
      void onExit(EntityID& id) override;
      void Update(EntityID& id1) override;
  };

  class ChaseEnemySerpentipede : public AIState
  {
  public:
      Vector2 DetectionRange;
      ChaseEnemySerpentipede(Vector2);
      void onEnter(EntityID& id) override;
      void onExit(EntityID& id) override;
      void Update(EntityID& id1) override;
  };
}
