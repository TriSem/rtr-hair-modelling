#pragma once
#include "Graphics.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

class SceneObject
{
public:

	virtual void Update();

	const Transform& GetTransform() const;

private:
	
	Transform transform;
};