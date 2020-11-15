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

  class IdleLancer : public AIState
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

  class IdleFiddler : public AIState
  {
    unsigned int CurrentWayPoint;
    Vector2 WayPoints[2];
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
}
