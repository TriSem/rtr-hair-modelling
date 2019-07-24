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
		void AddSceneObject(SceneObject& object);
		void DeleteSceneObject(int index);

		std::vector<SceneObject>& GetSceneObjects();

		Camera& GetCamera();

	private:

		std::vector<SceneObject> sceneObjects;
		Camera camera;
		Light light;
	};
}

