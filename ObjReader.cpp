#include "ObjReader.h"
#include <fstream>
#include "ErrorHandling.h"

using std::string;
using std::wstring;
using std::vector;


namespace OBJ
{
	ObjReader::ObjReader() :
		fileLoaded(false),
		primitiveMode(PrimitiveMode::TRIANGLE)
	{
		objects.push_back(Object());
	}

	ObjReader::~ObjReader()
	{
	}

	vector<Object> ObjReader::GetObjects() const
	{
		return objects;
	}

	bool ObjReader::LoadFile(const wstring& path)
	{
		wstring fileEnding = path.substr(path.length() - 4);
		if (fileEnding != L".obj")
		{
			OutputDebugString(L"Incorrect file type. File needs to be a wavefront obj file.");
			return false;
		}

		if (fileLoaded)
			UnloadFile();

		std::ifstream file;
		file.open(path.c_str());

		if (!file.is_open())
		{
			OutputDebugString(L"File could not be opened!\n");

			return false;
		}

		string line;
		while (std::getline(file, line))
		{
			if (line.empty())
				continue;

			char delimiter = ' ';
			size_t delimiterPosition = line.find(delimiter);

			string key = line.substr(0, delimiterPosition);
			string input = line.substr(delimiterPosition + 1, line.size() - delimiterPosition - 1);

			// Skip any unsupported commands.
			if (PARSING_OPS.find(key) == PARSING_OPS.end())
			{
				continue;
			}
			else
			{
				// Invoke the pointer to the class member function
				auto func = PARSING_OPS.at(key);
				(this->*func)(input);
			}
		}

		file.close();
		fileLoaded = true;
		return true;
	}

	void ObjReader::ReadVertex(const string& data)
	{
		vector<string> values = StringSplit(data, ' ');
		Vertex vertex;
		vertex.x = std::stof(values.at(0));
		vertex.y = std::stof(values.at(1));
		vertex.z = std::stof(values.at(2));

		if (values.size() == 4)
			vertex.w = std::stof(values.at(3));

		objects.back().modelData.vertices.push_back(vertex);
	}

	void ObjReader::ReadNormal(const string& data)
	{
		vector<string> values = StringSplit(data, ' ');
		Normal normal;
		normal.x = std::stof(values.at(0));
		normal.y = std::stof(values.at(1));
		normal.z = std::stof(values.at(2));

		objects.back().modelData.normals.push_back(normal);
	}

	void ObjReader::ReadTextureCoordinate(const string& data)
	{
		vector<string> values = StringSplit(data, ' ');
		TextureCoordinate textureCoordinate;
		textureCoordinate.u = std::stof(values.at(0));
		textureCoordinate.v = std::stof(values.at(1));

		objects.back().modelData.textureCoordinates.push_back(textureCoordinate);
	}

	void ObjReader::ReadFace(const string& data)
	{
		vector<string> valueGroups = StringSplit(data, ' ');
		Face face;

		int vertexCount = (primitiveMode == PrimitiveMode::TRIANGLE) ? 3 : 4;
		for (int i = 0; i < vertexCount; i++)
		{
			vector<string> values = StringSplit(valueGroups.at(i), '/');
			face.vertexIndices.push_back(std::stoi(values.at(0)));

			int numberOfSlashes = (int)std::count(values.begin(), values.end(), "/");
			if (numberOfSlashes == 1)
			{
				face.textureCoordinateIndices.push_back(std::stoi(values.at(1)));
			}
			else if (numberOfSlashes == 2)
			{
				if (values.at(1) != "")
					face.textureCoordinateIndices.push_back(std::stoi(values.at(1)));
				face.normalIndices.push_back(std::stoi(values.at(2)));
			}
		}

		objects.back().modelData.faces.push_back(face);
	}

	void ObjReader::ReadObjectName(const string& name)
	{
		if (objects.size() > 1)
			objects.push_back(Object(name));
		else
			objects.at(0).name = name;
	}

	void ObjReader::ReadMaterialName(const string& data)
	{
		// TODO: Implement.
	}

	void ObjReader::ReadMtlLib(const string& data)
	{
		// TODO: Implement.
	}

	vector<string> ObjReader::StringSplit(const string& str, char delimiter)
	{
		vector<string> strings;

		size_t start = 0;
		size_t delimiterPosition = str.find(delimiter, start);

		while (true)
		{
			if (delimiterPosition == string::npos)
			{
				strings.push_back(str.substr(start));
				return strings;
			}
			else
			{
				strings.push_back(str.substr(start, delimiterPosition - start));
				start = delimiterPosition + 1;
				delimiterPosition = str.find(delimiter, start);
			}
		}
	}

	void ObjReader::UnloadFile()
	{
		objects.clear();
		objects.push_back(Object());
	}

	/** Map of the supported obj statements. */
	const std::map<string, void (ObjReader::*)(const string&)> ObjReader::PARSING_OPS =
	{
		{"v", &ObjReader::ReadVertex},
		{"vt", &ObjReader::ReadTextureCoordinate},
		{"vn", &ObjReader::ReadNormal},
		{"f", &ObjReader::ReadFace},
		{"o", &ObjReader::ReadObjectName},
		{"usemtl", &ObjReader::ReadMaterialName},
		{"mtllib", &ObjReader::ReadMtlLib}
	};
}