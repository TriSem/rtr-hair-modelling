#pragma once
#include <vector>
#include "SceneObject.h"
#include "Camera.h"
#include "framework.h"
#include "Light.h"

class Scene
{
public:
	Scene();

	void Update();
	void AddSceneObject(const SceneObject& object);
	void DeleteSceneObject();

	Camera& GetCamera();

private:
	std::vector<SceneObject> sceneObjects;
	Camera camera;
	Light light;
};

