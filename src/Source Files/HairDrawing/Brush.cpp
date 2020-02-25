#include <Brush.h>

Brush::Brush()
{
}

void Brush::SetBrushMode(BrushMode mode)
{
	this->mode = mode;
}

BrushMode Brush::GetBrushMode()
{
	return mode;
}

void Brush::Paint()
{
}

void Brush::Erase()
{
}
