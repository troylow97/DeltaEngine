#include "CollisionManager.h"

namespace DeltaEngine
{
	CollisionManager::CollisionManager()
	{
		main_char = new SpriteRenderer{};
		main_char->transform.position.x = 5;
		main_char->transform.position.y = 5;
		main_char->transform.scale.x = 5;
		main_char->transform.scale.y = 5;
		//main_char->collider._aabb.min.x = main_char->transform.position.x - main_char->transform.scale.x / 2;
		//main_char->collider._aabb.min.y = main_char->transform.position.y - main_char->transform.scale.y / 2;
		//main_char->collider._aabb.max.x = main_char->transform.position.x + main_char->transform.scale.x / 2;
		//main_char->collider._aabb.max.y = main_char->transform.position.y + main_char->transform.scale.y / 2;
	}

	void CollisionManager::CollisionCheck()
	{
		//for (const SpriteRenderer* i : GameObjects)
		//{
		//	if (main_char->collider.Intersection(i->collider))
		//	{
		//		std::cout << "Collided" << std::endl;
		//	}
		//	else
		//	{
		//		std::cout << "NOTCollided" << std::endl;
		//	}
		//}
	}
}
