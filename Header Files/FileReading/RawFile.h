#pragma once
#include <string>
#include <fstream>
#include <iterator>
#include <array>
#include "Vertex.h"

class RawFile
{
	using byte = unsigned char;

public:
	RawFile();
	RawFile(std::string path);
	~RawFile();

	template <class DataType>
	std::vector<DataType> InterpretAsBuffer()
	{
		std::array<byte> buffer = ReadBuffer();
		std::vector<DataType> returnData;
		size_t typeSize = sizeof(DataType);
		for (int i = 0; i < buffer.size(); i += typeSize)
		{
			DataType value;
			std::array<byte, typeSize> bytes;
			for (int j = 0; j < typeSize; j++)
			{
				bytes.at(j) = buffer.at(i + j);
			}

			memcpy(&value, &bytes, sizeof(DataType));
			returnData.push_back(value);
		}

		return returnData;
	}


private:

	std::string path;
	std::shared_ptr<std::array<byte> > ReadBuffer();
};