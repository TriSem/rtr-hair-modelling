#include <Canvas.h>

using namespace DirectX::SimpleMath;

Canvas::Canvas(long width, long height) :
	area(DirectX::SimpleMath::Rectangle(0, 0, width, height))
{
}


void Canvas::TextureToRaw(std::string name)
{
}

void Canvas::LoadFromRaw(std::string path)
{
}

void Canvas::PaintInto(BrushData brushData, Vector2 brushPosition)
{
}

void Canvas::Clear()
{
	for (auto it = colors.begin(); it != colors.end(); it++)
	{
		*it = { 0, 0, 0, 1 };
	}
}
