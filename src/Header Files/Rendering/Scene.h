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
		void SetLight(DirectionalLight light);

		std::vector<std::shared_ptr<SceneObject>>& GetSceneObjects();

		Camera& GetCamera();
		DirectionalLight GetLight() const;

	private:

		std::vector<std::shared_ptr<SceneObject>> sceneObjects;
		Camera camera = Camera();
		DirectionalLight light = DirectionalLight();
	};
}

