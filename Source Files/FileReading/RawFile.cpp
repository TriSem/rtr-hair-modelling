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

std::vector<char> RawFile::ReadBuffer()
{
	std::ifstream stream(path, std::ios::in | std::ios::binary);

	assert(stream.is_open());

	char numberOfWordsBinary[4];
	stream.read(&numberOfWordsBinary[0], 4);

	unsigned int numberOfWords = 0;
	memcpy(&numberOfWords, numberOfWordsBinary, sizeof(unsigned int));
	size_t numberOfBytes = numberOfWords * sizeof(float);
	std::vector<char> buffer;

	stream.seekg(4, std::ios::beg);
	stream.read(&buffer[0], numberOfBytes);
	stream.close();

	return buffer;
}