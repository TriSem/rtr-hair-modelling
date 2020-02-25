#pragma once

enum class BrushMode;

class Brush
{
public:

	Brush();

	void SetBrushMode(BrushMode mode);
	BrushMode GetBrushMode();
	void Paint();
	void Erase();

private:

	float radius = 0.5f;
	int strength = 10;
	int strengthChange = 5;
	BrushMode mode = BrushMode::Length;
};

enum class BrushMode
{
	Length,
	Curl,
	Twist
};