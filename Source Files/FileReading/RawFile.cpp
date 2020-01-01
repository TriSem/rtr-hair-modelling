#include "RawFile.h"


RawFile::RawFile() :
	path("")
{
}

RawFile::RawFile(std::string path) :
	path(path)
{
}

RawFile::~RawFile()
{
}

std::vector<Color> RawFile::InterpretAsTexture()
{
	std::ifstream stream(path, std::ios::in | std::ios::binary);
	assert(stream.is_open());

	std::vector<byte> bytes;
	bytes.insert(bytes.begin(), std::istream_iterator<byte>(stream), std::istream_iterator<byte>());

	std::vector<Color> texture;
	texture.reserve(bytes.size());

	for (uint32_t i = 0; i < bytes.size(); i += sizeof(Color))
	{
		Color color = {};
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

	char numberOfWordsBinary[4];
	stream.read(&numberOfWordsBinary[0], 4);

	unsigned int numberOfWords = 0;
	memcpy(&numberOfWords, numberOfWordsBinary, sizeof(unsigned int));
	size_t numberOfBytes = numberOfWords * sizeof(float);
	std::vector<byte> buffer;

	stream.seekg(4, std::ios::beg);
	stream.read((char*) &buffer[0], numberOfBytes);
	stream.close();

	return buffer;
}