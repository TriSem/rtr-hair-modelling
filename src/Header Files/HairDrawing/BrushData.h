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
	int strength;
	float radius;
	PaintChannel paintChannel;
	BrushMode mode;
};