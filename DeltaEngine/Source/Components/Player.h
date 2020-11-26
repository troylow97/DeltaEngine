#pragma once

#include <Core\Math\Vector.h>

namespace DeltaEngine
{
	struct Player
	{
		int RespawnPoint;
		bool IsDead = false;
		//std::vector<Vector2> m_respawn_points;

		//Player() /*:
		//	m_respawn_points{ Vector2{0,0}, Vector2{0,0}, 
		//	                  Vector2{0,0}, Vector2{0,0}, Vector2{0,0} }*/
		//{
		//	JsonFile file;
		//	rttr::variant v{ m_respawn_points };
		//	auto& seq = v.create_sequential_view();
		//	file.StartWriter("player/respawn_points.json").StartObject().WriteObject(m_respawn_points).EndObject();
		//}
	};
}