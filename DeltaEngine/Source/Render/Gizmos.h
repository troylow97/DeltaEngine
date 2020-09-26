#pragma once

#include "Core/Math/Transform.h"
#include "Core/Math/Color.h"

namespace DeltaEngine::Gizmos
{
	//Color color;

	void Init();
	void Exit();

	void SetColor(Color color = Color::green());

	void DrawWorldGrid();

	void Draw2DBox(Vector3 position, Quaternion rotation = Quaternion::identity(), Vector3 scale = Vector3::one());
	void Draw2DBox(Transform transform = Transform());

	void Draw2DWireBox(Vector3 position, Quaternion rotation = Quaternion::identity(), Vector3 scale = Vector3::one());
	void Draw2DWireBox(Transform transform = Transform());

	void Draw2DCircle(Vector3 position, Quaternion rotation = Quaternion::identity(), Vector3 scale = Vector3::one());
	void Draw2DCircle(Transform transform = Transform());

	void Draw2DWireCircle(Vector3 position, Quaternion rotation = Quaternion::identity(), Vector3 scale = Vector3::one());
	void Draw2DWireCircle(Transform transform = Transform());
}
