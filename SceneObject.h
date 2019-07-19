#pragma once
#include "Graphics.h"

using namespace DirectX::SimpleMath;

class SceneObject
{
public:

	virtual void Update();

private:
	Vector3 positition;
	Quaternion rotation;
	Vector3 scale;
};