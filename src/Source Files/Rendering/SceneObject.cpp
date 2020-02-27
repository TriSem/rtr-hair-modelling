#include "SceneObject.h"

namespace Rendering
{
	SceneObject::SceneObject(std::shared_ptr<IMesh> mesh) :
		mesh(mesh)
	{
	}

	void SceneObject::Update()
	{
	}

	Transform& SceneObject::GetTransform()
	{
		return transform;
	}

	std::shared_ptr<IMesh> SceneObject::GetMesh()
	{
		return mesh;
	}

	void SceneObject::IssueRenderCommands()
	{
		if (mesh != nullptr)
			mesh->IssueRenderCommands();

		if (material != nullptr)
			material->IssueRenderCommands();
	}
}
