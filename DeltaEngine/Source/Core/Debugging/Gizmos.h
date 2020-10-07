#pragma once

#include "Core/Math/Transform.h"
#include "Core/Math/Color.h"
#include "Physics/Collider.h"

namespace DeltaEngine::Gizmos
{
	//Color color;
	void Init();
	void Exit();

	void SetColor(Color color = Color::green());

	void DrawWorldGrid();

	void Draw2DBox(Vector3 position, Vector3 scale = Vector3::one(), Quaternion rotation = Quaternion::identity());
	void Draw2DBox(Transform transform = Transform());

	void Draw2DWireBox(Vector3 position, Vector3 scale = Vector3::one(), Quaternion rotation = Quaternion::identity());
	void Draw2DWireBox(Transform transform = Transform());

	void Draw2DCircle(Vector3 position, Vector3 scale = Vector3::one(), Quaternion rotation = Quaternion::identity());
	void Draw2DCircle(Transform transform = Transform());

	void Draw2DWireCircle(Vector3 position, Vector3 scale = Vector3::one(), Quaternion rotation = Quaternion::identity());
	void Draw2DWireCircle(Transform transform = Transform());

    //ERROR: NEED TO FIX
	//void OnDrawGizmos(Collider& col);
}
