#pragma once
#include <string>
#include <fstream>
#include <iterator>
#include <array>
#include <vector>
#include "Vertex.h"
#include "Color.h"

class RawFile
{
	using byte = unsigned char;

public:
	RawFile(std::string path);
	~RawFile();

	template <class DataType>
	std::vector<DataType> InterpretAsBuffer()
	{
		std::vector<byte> buffer = ReadBuffer();
		std::vector<DataType> returnData;
		size_t typeSize = sizeof(DataType);
		for (int i = 0; i < buffer.size(); i += typeSize)
		{
			DataType value;
			std::vector<byte> bytes;
			for (int j = 0; j < typeSize; j++)
			{
				bytes.push_back(buffer.at(i + j));
			}

			memcpy(&value, &bytes, sizeof(DataType));
			returnData.push_back(value);
		}

		return returnData;
	}

	std::vector<Rendering::Color> InterpretAsTexture();

private:

	std::string path;
	std::vector<byte> ReadBuffer();
};