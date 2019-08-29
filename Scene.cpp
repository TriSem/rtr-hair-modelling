#include "Scene.h"

namespace Rendering
{
	Scene::Scene() :
		camera(Camera()),
		light(Light())
	{
	
	}

	void Scene::Update()
	{
		if(!sceneObjects.empty())
		for (SceneObject object : sceneObjects)
		{
			object.Update();
		}
	}

	void Scene::AddSceneObject(SceneObject& object)
	{
		sceneObjects.push_back(object);
	}

	void Scene::DeleteSceneObject(int index)
	{
		sceneObjects.erase(sceneObjects.begin() + index);
	}

	std::vector<SceneObject>& Scene::GetSceneObjects()
	{
		return sceneObjects;
	}

	Camera& Scene::GetCamera()
	{
		return camera;
	}
}
