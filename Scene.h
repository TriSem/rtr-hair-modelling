#pragma once
#include <vector>
#include "SceneObject.h"
#include "Camera.h"
#include "framework.h"
#include "Light.h"

class Scene
{
public:
	void Update();

private:
	std::vector<SceneObject> sceneObjects;
	std::shared_ptr<Camera> camera;
	std::vector<Light> light;
};

