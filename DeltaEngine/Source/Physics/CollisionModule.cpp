#include "Physics/CollisionModule.h"

namespace DeltaEngine
{
	CollisionModule::CollisionModule()
	{
		main_char = new SpriteRenderer{};
		main_char->transform.position.x = 5;
		main_char->transform.position.y = 5;
		main_char->transform.scale.x = 5;
		main_char->transform.scale.y = 5;
		main_char->collider._aabb.min.x = main_char->transform.position.x - main_char->transform.scale.x / 2;
		main_char->collider._aabb.min.y = main_char->transform.position.y - main_char->transform.scale.y / 2;
		main_char->collider._aabb.max.x = main_char->transform.position.x + main_char->transform.scale.x / 2;
		main_char->collider._aabb.max.y = main_char->transform.position.y + main_char->transform.scale.y / 2;
	}

	void CollisionModule::CollisionCheck()
	{
		for (const SpriteRenderer* i : GameObjects)
		{
			if (main_char->collider.Intersection(i->collider))
			{
				std::cout << "Collided" << std::endl;
				//std::pair<Collider, Collider> ColliderPair(main_char->collider, i->collider);
				//Event Messaging system add into a queue, have collision handler resolve

			}
			else
			{
				std::cout << "NOTCollided" << std::endl;
			}
		}
	}

	void CollisionModule::Update()
	{

	}
}
