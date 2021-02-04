/**********************************************************************************
* \file   ICloneWorld.h
* \brief  This file contains the implementation of XXXXXXXXXXXXX
*
* \author XXX, XXXXXXXX, X% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace DeltaEngine
{
  class ICloneWorld // interface class
  {
  public:
    virtual ICloneWorld* clone() = 0;
    // if a class has anything virtual, it must ALWAYS have a virtual destructor
    virtual ~ICloneWorld() = default; 
    /*
      // virtual as it is an interface 
      virtual void attack() = 0; 
      virtual IWeapon* clone() = 0;
      virtual ~IWeapon() = default;
      // if a class has anything virtual, it must ALWAYS have a virtual destructor
    */
  };
  /* -----------------------------------------------------------------------------------
	class WorldFactory
	{
	  std::map<int, ICloneWorld*> _m_worlds;
	  //std::vector<World> _m_worlds;
	public:
	  ICloneWorld* create(int world_index)
	  {
	  	std::map<int, ICloneWorld*>::iterator it = _m_worlds.find(world_index);
	  
	  	if (it != _m_worlds.end())
	  	{
	  	  return it->second->clone();
	  	}
	  }
	  void addPrototype(int world_index, ICloneWorld* world)
	  {
	    _m_worlds[world_index] = world;

		// ------------------------------ to clone
		// Factory factory;
		// factory.printPrototypes();
		// factory.addPrototype(0, &staff_prototype);
		// factory.addPrototype(1, &bow_prototype);
		// factory.addPrototype(2, &superbow_prototype);
	  }
	};
	----------------------------------------------------------------------------------- */
}
/* --------------------------------------------------------------------------------------------
play pause stop
	- need to integrate with the system spencer wrote
	- settle a part in the architecture
	- duplicate the world, copy the exact copy of the world
	- press play duplicate a copy of editor world into another world (game world)

	- an addition to the class ECSModule to copy world (get Entities Component Data 
	  from m_world[0], copy Entities Component Data to m_world[1])
	- link Play, Pause, Stop
	- update the logic sequencing if required



protected:
  virtual World* clone_impl() const = 0;
  virtual ~World() = default; // if a class has anything virtual, it must ALWAYS have a virtual destructor

public:
  auto clone() const
  {
    return std::unique_ptr<World>(clone_impl());
  }
-------------------------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------------------------
	when world is constructed, put in the ECS module inside, 
	   can create a reference for world
	independency injection

	m_game_world.reserve(m_worlds.size());

	for (const auto& i : m_worlds)
	  m_game_world.push_back(std::make_unique<World>());
-------------------------------------------------------------------------------------------- */