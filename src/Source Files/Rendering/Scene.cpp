#include "Scene.h"

namespace Rendering
{
	Scene::Scene()
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

	void Scene::AddSceneObject(shared_ptr<SceneObject> object)
	{
		sceneObjects.push_back(object);
	}

	void Scene::DeleteSceneObject(int index)
	{
		sceneObjects.erase(sceneObjects.begin() + index);
	}

	void Scene::SetLight(DirectionalLight light)
	{
		this->light = light;
	}

	std::vector<shared_ptr<SceneObject>>& Scene::GetSceneObjects()
	{
		return sceneObjects;
	}

	Camera& Scene::GetCamera()
	{
		return camera;
	}
	DirectionalLight Scene::GetLight() const
	{
		return light;
	}
}
