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
		/*for (SceneObject object : sceneObjects)
		{
			object.Update();
		}*/
	}

	void Scene::AddSceneObject(const SceneObject& object)
	{
	
	}

	void Scene::DeleteSceneObject()
	{
	}

	Camera& Scene::GetCamera()
	{
		return camera;
	}
}
