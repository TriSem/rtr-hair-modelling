#include "RawFile.h"


RawFile::RawFile(std::string path) :
	path(path)
{
}

RawFile::~RawFile()
{
}

std::vector<Rendering::Color> RawFile::InterpretAsTexture()
{
	std::ifstream stream(path, std::ios::in | std::ios::binary | std::ios::ate);
	assert(stream.is_open());
	stream.unsetf(std::ios::skipws);
	size_t numberOfBytes = stream.tellg();
	stream.seekg(std::ios::beg);

	std::vector<byte> bytes(numberOfBytes);
	stream.read(bytes.data(), numberOfBytes);
	stream.close();

	size_t numberOfPixels = bytes.size() / 3;
	std::vector<Rendering::Color> texture(numberOfPixels);

	for (uint32_t i = 0; i < numberOfPixels; i++)
	{
		size_t triple = i * (size_t)3;
		Rendering::Color color = {};
		color.red = bytes.at(triple);
		color.green = bytes.at(triple + 1);
		color.blue = bytes.at(triple + 2);
		color.alpha = 255;
		texture[i] = color;
	}

	return texture;
}

std::vector<RawFile::byte> RawFile::ReadBuffer()
{
	std::ifstream stream(path, std::ios::in | std::ios::binary);
	assert(stream.is_open());
	stream.unsetf(std::ios::skipws);

	byte numberOfWordsBinary[4];
	stream.read(&numberOfWordsBinary[0], 4);

	unsigned int numberOfWords = 0;
	memcpy(&numberOfWords, numberOfWordsBinary, sizeof(unsigned int));

	size_t numberOfBytes = numberOfWords * sizeof(float);
	std::vector<byte> bytes(numberOfBytes);

	stream.seekg(4, std::ios::beg);
	stream.read(bytes.data(), numberOfBytes);
	stream.close();

	return bytes;
}