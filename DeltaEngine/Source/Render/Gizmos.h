#pragma once

#include "Math/Transform.h"
#include "Math/Color.h"

namespace DeltaEngine
{
	namespace Gizmos
	{
		void Init();
		void DrawWorldGrid();

		void Draw2DBox(Vector3 position, Quaternion rotation = Quaternion::identity(), Vector3 scale = Vector3::one());
		void Draw2DBox(Transform transform = Transform());
		
		void Draw2DBoxWire(Vector3 position, Quaternion rotation = Quaternion::identity(), Vector3 scale = Vector3::one());
		void Draw2DBoxWire(Transform transform = Transform());
		
		void DrawCube(Vector3 position, Quaternion rotation = Quaternion(), Vector3 scale = Vector3::one());
		void DrawCube(Transform transform = Transform());
		
		void DrawCubeWire(Vector3 position, Quaternion rotation = Quaternion(), Vector3 scale = Vector3::one());
		void DrawCubeWire(Transform transform = Transform());
		
		void DrawSphere(Vector3 position, Quaternion rotation = Quaternion(), Vector3 scale = Vector3::one());
		void DrawSphere(Transform transform = Transform());

		void DrawSphereWire(Vector3 position, Quaternion rotation = Quaternion(), Vector3 scale = Vector3::one());
		void DrawSphereWire(Transform transform = Transform());
	};
}
