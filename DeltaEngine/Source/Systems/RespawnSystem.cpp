#include "Systems/RespawnSystem.h"

namespace DeltaEngine
{
  void RespawnSystem::Initialize()
  {
  	//RespawnPoints res;
  	//res.m_respawns.push_back({ 0,0 });
  	
  	JsonFile file;
  	//rttr::variant v{ res };
  	//auto& seq = v.create_sequential_view();
  	//file.StartWriter("Player/respawn_points.json").StartObject().WriteObject(v).EndObject().EndWriter();
  
  	file.StartReader("Player/respawn_points.json").LoadObject(respawns).EndReader();
  
  }
  
  void RespawnSystem::Update()
  {
  	em.ForEach([&](EntityID& id, Health& hp, EntityType& et, Transform& t, Player& p)
  	{
      //if (p.IsDead)
      //{
      //  for (int i = 0; i < respawns.m_respawns.size(); i++)
      //  {
      //    float temp_x = 0.0f, temp_y = 0.0f;
      //
      //    if (t.position.x >= respawns.m_respawns[i].x)
      //    {
      //      temp_x = respawns.m_respawns[i].x;
      //      temp_y = respawns.m_respawns[i].y;
      //    }
      //    t.position.x = temp_x;
      //    t.position.y = temp_y;
      //  }
      //}
      //t.position = respawns.m_respawns[p.RespawnPoint]; // checks if player is dead. if dead set pos to current one
  	});
  }
  void RespawnSystem::LateUpdate()
  {
  
  }
}