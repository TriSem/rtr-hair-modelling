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

std::shared_ptr<std::array<byte> > RawFile::ReadBuffer()
{
	std::ifstream stream(path, std::ios::in | std::ios::binary);

	assert(stream.is_open());

	byte numberOfWordsBinary[4];
	stream.read(numberOfWordsBinary, 4);

	unsigned int numberOfWords = (unsigned int)numberOfWordsBinary;
	size_t numberOfBytes = numberOfWords * sizeof(float);
	std::shared_ptr<std::array<byte, numberOfBytes> > buffer();

	stream.seekg(4, ios::beg);
	stream.read(buffer, numberOfBytes);
	stream.close();

	return buffer;
}