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
		if (!sceneObjects.empty())
		{
			for (auto object : sceneObjects)
			{
				object->Update();
			}
		}
	}

	void Scene::AddSceneObject(std::shared_ptr<SceneObject> object)
	{
		sceneObjects.push_back(object);
	}

	void Scene::DeleteSceneObject(int index)
	{
		sceneObjects.erase(sceneObjects.begin() + index);
	}

	std::vector<std::shared_ptr<SceneObject>>& Scene::GetSceneObjects()
	{
		return sceneObjects;
	}

	Camera& Scene::GetCamera()
	{
		return camera;
	}
}
