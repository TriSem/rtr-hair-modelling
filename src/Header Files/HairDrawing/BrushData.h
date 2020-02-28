enum class PaintChannel
{
	Length,
	Curl,
	Twist
};

enum class BrushMode
{
	Paint,
	Erase
};

struct BrushData
{
	float strength;
	float radius;
	PaintChannel paintChannel;
	BrushMode mode;
};