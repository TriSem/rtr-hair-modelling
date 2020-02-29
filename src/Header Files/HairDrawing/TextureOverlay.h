#pragma once
#include <framework.h>
#include <SceneObject.h>

class TextureOverlay : public Rendering::SceneObject
{
public:
    
    TextureOverlay();
    TextureOverlay(shared_ptr<Rendering::Mesh> mesh);

protected:

    virtual void Update() override;
};