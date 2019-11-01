#pragma once
#include <string>
#include <fstream>
#include <iterator>
#include <array>

class RawFile
{
	using byte = unsigned char;

public:
	RawFile();
	RawFile(std::string path);
	~RawFile();

	std::shared_ptr<std::array<byte> > ReadBuffer();
	void InterpretAsTexture();

private:

	std::string path;
	int BytesToInt(byte integer[4]);
	float BytesToFloat(byte floatingPoint[4]);


};