#pragma once

#include "House/House.h"
#include "House/Room.h"
#include "Helpers/Util.h"

#include "rapidjson/document.h"
#include <rapidjson/filereadstream.h>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <fstream>
#include <string>
#include <iostream>

namespace HellEngine {

	class File
	{
	public: // static fields
		static std::vector<std::string> MapList;

	public: // static methods
		static void LoadMapNames();
		static House LoadMap(std::string filename);
		static void SaveMap(std::string filename, House* house);
		
		static void SaveVec2(rapidjson::Value* object, std::string elementName, glm::vec2 vector, rapidjson::Document::AllocatorType& allocator);
		static void SaveVec3(rapidjson::Value* object, std::string elementName, glm::vec3 vector, rapidjson::Document::AllocatorType& allocator);
		static void SaveString(rapidjson::Value* object, std::string elementName, std::string string, rapidjson::Document::AllocatorType& allocator);
		static void SaveBool(rapidjson::Value* object, std::string elementName, bool boolean, rapidjson::Document::AllocatorType& allocator);
		static void SaveFloat(rapidjson::Value* object, std::string elementName, float number, rapidjson::Document::AllocatorType& allocator);
		static void SaveInt(rapidjson::Value* object, std::string elementName, int number, rapidjson::Document::AllocatorType& allocator);

		static glm::vec2 ReadVec2(const rapidjson::Value& value, std::string name);
		static glm::vec3 ReadVec3(const rapidjson::Value& value, std::string name);
		static std::string ReadString(const rapidjson::Value& value, std::string name);
		static bool ReadBool(const rapidjson::Value& value, std::string name);
		static float ReadFloat(const rapidjson::Value& value, std::string name);
		static int ReadInt(const rapidjson::Value& value, std::string name);
	};
}
