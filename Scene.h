#pragma once
#include <vector>
#include "SceneObject.h"
#include "Camera.h"
#include "framework.h"
#include "Light.h"

namespace Rendering
{
	class Scene
	{
	public:
		Scene();

		void Update();
		void AddSceneObject(std::shared_ptr<SceneObject> object);
		void DeleteSceneObject(int index);

		std::vector<std::shared_ptr<SceneObject>>& GetSceneObjects();

		Camera& GetCamera();

	private:

		int a = 0;
		std::vector<std::shared_ptr<SceneObject>> sceneObjects;
		Camera camera;
		Light light;
	};
}

