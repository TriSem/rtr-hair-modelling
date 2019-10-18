#pragma once

class RawReader
{
public:
	RawReader();
	~RawReader();


private:

	void ReadVertexData();
	void ReadIndexData();
	void ReadTextureData();
};