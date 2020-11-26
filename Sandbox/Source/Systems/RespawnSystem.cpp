#include "../../Sandbox/Source/Systems/RespawnSystem.h"
#include "AI/AITools.h"

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
      if (p.IsDead)
      {
        float temp_x = -4.149f, temp_y = -2.68f;
        float new_x = 0.0f, new_y = 0.0f;
        if (AITools::isFacingRight(id))
        {
          for (size_t i = 0; i < respawns.m_respawns.size(); i++)
          {
            if (t.position.x >= respawns.m_respawns[i].x)
            {
              temp_x = respawns.m_respawns[i].x;
              temp_y = respawns.m_respawns[i].y;
            }
          }
          new_x = temp_x;
          new_y = temp_y;
          t.position.x = new_x;
          t.position.y = new_y;
          hp.CurrentHealth = hp.MaxHealth;
          p.IsDead = false;
        }
        else if (AITools::isFacingLeft(id))
        {
          for (size_t i = respawns.m_respawns.size(); i > 0; i--)
          {
            if (t.position.x <= respawns.m_respawns[i].x)
            {
              temp_x = respawns.m_respawns[i].x;
              temp_y = respawns.m_respawns[i].y;
            }
          }
          new_x = temp_x;
          new_y = temp_y;
          t.position.x = new_x;
          t.position.y = new_y;
          hp.CurrentHealth = hp.MaxHealth;
          p.IsDead = false;
        }
      }
    });
  }
  void RespawnSystem::LateUpdate()
  {

  }
}