#pragma once

#include "ObjData.h"
#include <iostream>
#include <map>
#include <string>
#include <ppltasks.h>

namespace OBJ
{
	class ObjReader
	{
	private:
		static const std::map<std::string, void (ObjReader::*)(const std::string&)> PARSING_OPS;

		PrimitiveMode primitiveMode;

		std::vector<Object> objects;
		bool fileLoaded;

		void ReadVertex(const std::string& data);
		void ReadNormal(const std::string& data);
		void ReadTextureCoordinate(const std::string& data);
		void ReadFace(const std::string& data);
		void ReadObjectName(const std::string& name);
		void ReadMaterialName(const std::string& name);
		void ReadMtlLib(const std::string& data);

		std::vector<std::string> StringSplit(const std::string& str, char delimiter);


	public:
		ObjReader();
		~ObjReader();

		std::vector<Object> GetObjects() const;

		bool LoadFile(const std::string& path);
		void UnloadFile();
	};
}