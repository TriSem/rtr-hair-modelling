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
	std::ifstream stream(path, std::ios::in | std::ios::binary);
	assert(stream.is_open());
	stream.unsetf(std::ios::skipws);

	std::vector<byte> bytes;
	bytes.insert(bytes.begin(), std::istream_iterator<byte>(stream), std::istream_iterator<byte>());

	std::vector<Rendering::Color> texture;
	texture.reserve(bytes.size());

	for (uint32_t i = 0; i < bytes.size(); i += sizeof(Rendering::Color))
	{
		Rendering::Color color = {};
		color.red = bytes.at(i);
		color.green = bytes.at(i + 1);
		color.blue = bytes.at(i + 2);
		texture.push_back(color);
	}

	return texture;
}

std::vector<byte> RawFile::ReadBuffer()
{
	std::ifstream stream(path, std::ios::in | std::ios::binary);
	assert(stream.is_open());
	stream.unsetf(std::ios::skipws);

	char numberOfWordsBinary[4];
	stream.read(&numberOfWordsBinary[0], 4);

	unsigned int numberOfWords = 0;
	memcpy(&numberOfWords, numberOfWordsBinary, sizeof(unsigned int));
	size_t numberOfBytes = numberOfWords * sizeof(float);
	std::vector<byte> buffer;
	buffer.reserve(numberOfBytes);

	std::istream_iterator<byte> start(stream);
	buffer.insert(buffer.begin(), start, std::istream_iterator<byte>());
	stream.close();

	return buffer;
}