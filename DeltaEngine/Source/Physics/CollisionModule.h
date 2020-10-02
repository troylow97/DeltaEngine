#pragma once
#include "Render/SpriteRenderer.h" //for testing
#include <vector>
#include "Physics/Collider.h"

namespace DeltaEngine
{
	class DE_API CollisionModule
	{
	public:
		CollisionModule();
		void CollisionCheck();
		void Update();

	public:
		SpriteRenderer* main_char;
		std::vector<SpriteRenderer*> GameObjects;
		//std::vector<ActionList<>> Collision;
	};

}
