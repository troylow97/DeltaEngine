#pragma once
#include "Collision.h"
#include "Collider.h"
#include "ActionList.h"
#include "../Render/SpriteRenderer.h" //for testing
#include <vector>

namespace DeltaEngine
{

	class DE_API CollisionManager
	{
	public:
		CollisionManager();
		void CollisionCheck();

	public:
		SpriteRenderer* main_char;
		std::vector<SpriteRenderer*> GameObjects;
		//std::vector<Collider> ColliderList;
		//ActionList<> CollisionResolutions;
	};

}
