#include <Canvas.h>

using namespace DirectX::SimpleMath;

Canvas::Canvas()
{
}


void Canvas::TextureToRaw(std::string name)
{
}

void Canvas::LoadFromRaw(std::string path)
{
}


void Canvas::Clear()
{
	for (auto it = colors.begin(); it != colors.end(); it++)
	{
		*it = { 0, 0, 0, 1 };
	}
}
