#pragma once
#include <string>
#include <fstream>
#include <iterator>
#include <array>
#include <vector>
#include "Vertex.h"
#include "Color.h"

using DirectX::SimpleMath::Color;

class RawFile
{
	using byte = char;

public:
	RawFile(std::string path);
	~RawFile();

	template <class DataType>
	std::vector<DataType> InterpretAsBuffer()
	{
		std::vector<byte> buffer = ReadBuffer();
		std::vector<DataType> returnData;
		const size_t typeSize = sizeof(DataType);
		for (size_t i = 0; i < buffer.size(); i += typeSize)
		{
			DataType value;
			byte bytes[typeSize];
			for (size_t j = 0; j < typeSize; j++)
			{
				bytes[j] = buffer.at(i + j);
			}

			memcpy(&value, &bytes, sizeof(DataType));
			returnData.push_back(value);
		}

		return returnData;
	}

	std::vector<Color> InterpretAsTexture();

private:

	std::string path;
	std::vector<byte> ReadBuffer();
};