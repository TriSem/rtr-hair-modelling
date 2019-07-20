#include "Scene.h"

Scene::Scene()
{
	camera = std::make_unique<Camera>(ProjectionMode::PERSPECTIVE);
}

void Scene::Update()
{

}