#pragma once

#include "ECS/Components/Transform.h"
#include "Core/Math/Color.h"


namespace DeltaEngine::Gizmos
{
	//Color color;
	void Init();
	void Exit();

	void SetColor(Color color = Color::Green());

	void DrawWorldGrid();

	void DrawLine(Vector3 start, Vector3 end);

	void Draw2DBox(Vector3 position, Vector3 scale = Vector3::one(), Quaternion rotation = Quaternion::Identity());
	void Draw2DBox(Transform transform = Transform());

	void Draw2DWireBox(Vector3 position, Vector3 scale = Vector3::one(), Quaternion rotation = Quaternion::Identity());
	void Draw2DWireBox(Transform transform = Transform());

	void Draw2DCircle(Vector3 position, Vector3 scale = Vector3::one(), Quaternion rotation = Quaternion::Identity());
	void Draw2DCircle(Transform transform = Transform());

	void Draw2DWireCircle(Vector3 position, Vector3 scale = Vector3::one(), Quaternion rotation = Quaternion::Identity());
	void Draw2DWireCircle(Transform transform = Transform());

}
