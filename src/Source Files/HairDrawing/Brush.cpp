#include <Brush.h>

Brush::Brush(std::shared_ptr<Canvas> canvas) :
	canvas(canvas)
{

}

void Brush::SetPaintChannel(PaintChannel channel)
{
	data.paintChannel = channel;
}

PaintChannel Brush::GetPaintChannel()
{
	return data.paintChannel;
}

void Brush::Paint(Vector2 position)
{
	data.mode = BrushMode::Paint;
	canvas->PaintInto(data, position);
}

void Brush::Erase(Vector2 position)
{
	data.mode = BrushMode::Erase;
	canvas->PaintInto(data, position);
}

void Brush::IncreaseStrength()
{
	data.strength += strengthChange;
}

void Brush::DecreaseStrength()
{
	data.strength -= strengthChange;
}
