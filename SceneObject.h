#pragma once
#include "Graphics.h"
#include "Transform.h"
#include "GeometryDataStructs.h"
#include <memory>

using namespace DirectX::SimpleMath;

class SceneObject
{
public:

	virtual void Update();

	const Transform& GetTransform() const;

private:
	
	Transform transform;
	std::shared_ptr<Mesh> mesh;
};