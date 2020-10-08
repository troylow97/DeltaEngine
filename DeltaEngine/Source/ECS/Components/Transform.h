#pragma once
#include "Core/Math/DE_Math.h"
#include "DE_API.h"
#include <rttr/registration>
#include <ECS/EntityManager.h>
#include "Core/Utils/Json/JsonFile.h"
namespace DeltaEngine
{
	class DE_API Transform
	{
	public:
		Transform* parent;
		Vector3 old_position;
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
		Transform();
		Transform(Vector3 position, Quaternion rotation, Vector3 scale);
		Matrix4x4 LocalToWorldMatrix(bool withParents = true);

		static void Serialize(JsonFile& file, void* ptr)
		{
			file.Write( *static_cast<Transform *>( ptr ) );
		}

		static void Deserialize(EntityManager& em, EntityID id, rttr::variant var)
		{
			Transform t = var.get_value<Transform>();
			em.add_component<Transform>( id, t );
		}
	};

}