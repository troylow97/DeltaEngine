#pragma once

#include "Mesh.h"
#include "Math/Transform.h"
#include "Math/Color.h"
#include "Texture.h"
#include "Shader.h"

class SpriteRenderer
{
public:
	Mesh* mesh;
	Texture2D* sprite;
	Shader* shader;
	Transform transform;
	Color color;
	SpriteRenderer();
	~SpriteRenderer();
	void Update();
};
