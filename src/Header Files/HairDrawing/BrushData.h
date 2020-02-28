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
	uint8_t strength;
	float radius;
	PaintChannel paintChannel;
	BrushMode mode;
};