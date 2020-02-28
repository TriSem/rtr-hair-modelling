#pragma once
#include <framework.h>
#include <SceneObject.h>

class TextureOverlay : public Rendering::SceneObject
{
public:
    
    TextureOverlay();
    TextureOverlay(std::shared_ptr<Rendering::Mesh> mesh);
};