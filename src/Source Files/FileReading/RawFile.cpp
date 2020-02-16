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

	size_t numberOfPixels = bytes.size() / 4;
	std::vector<Rendering::Color> texture(numberOfPixels);

	static_assert(std::is_trivially_copyable<Rendering::Color>::value,
		"Rendering must be trivially copyable");

	for (uint32_t i = 0; i < numberOfPixels; i += sizeof(Rendering::Color))
	{
		Rendering::Color color = {};
		color.red = bytes.at(i);
		color.green = bytes.at(i + (size_t)1);
		color.blue = bytes.at(i + (size_t)2);
		color.alpha = 255;
		texture.push_back(color);
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
	std::vector<byte> buffer;
	buffer.reserve(numberOfBytes);

	std::istream_iterator<byte> start(stream);
	buffer.insert(buffer.begin(), start, std::istream_iterator<byte>());
	stream.close();

	return buffer;
}