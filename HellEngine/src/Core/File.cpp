#include "hellpch.h"
#include "File.h"
#include <filesystem>
#include <sstream> 
#include <Helpers/Util.h> 

namespace HellEngine {

	std::vector<std::string> File::MapList;

	void File::LoadMapNames()
	{
		// Look for maps
		std::string path = "res/maps/";
		for (const auto & entry : std::filesystem::directory_iterator(path))
		{
			std::stringstream ss;
			ss << entry.path();
			std::string filename = ss.str();
			filename = Util::RemoveCharactersFromEnd(filename, 1);
			filename = Util::RemoveCharactersFromBeginning(filename, path.length() + 1);
			MapList.push_back(filename);
		}
	}

	House File::LoadMap(std::string filename)
	{
		// Load file
		std::string fileName = "res/maps/" + filename;
		FILE* pFile = fopen(fileName.c_str(), "rb");
		char buffer[65536];
		rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
		rapidjson::Document document;
		document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

		// Check for errors
		if (document.HasParseError())
			std::cout << "Error  : " << document.GetParseError() << '\n' << "Offset : " << document.GetErrorOffset() << '\n';

		// Hardcode type names
		static const char* kTypeNames[] =
		{ "Null", "False", "True", "Object", "Array", "String", "Number" };

		House house;
		//house.position = glm::vec3(0, 0, 0);

		// Rooms
		if (document.HasMember("ROOMS"))
		{
			const rapidjson::Value& rooms = document["ROOMS"];
			for (rapidjson::SizeType i = 0; i < rooms.Size(); i++)
			{
				glm::vec2 position = ReadVec2(rooms[i], "Position");
				glm::vec2 size = ReadVec2(rooms[i], "Size");
				int story = ReadInt(rooms[i], "Story");
				house.AddRoom(Room(position, size, story, &house));
			}
		}

		// Doors
		if (document.HasMember("DOORS"))
		{
			const rapidjson::Value& rooms = document["DOORS"];
			for (rapidjson::SizeType i = 0; i < rooms.Size(); i++)
			{
				glm::vec3 position = ReadVec3(rooms[i], "Position");
				std::string axis = ReadString(rooms[i], "Axis");
				int story = ReadInt(rooms[i], "Story");
				bool rotateFloor = ReadBool(rooms[i], "RotateFloorTexture");

				house.AddDoor(Door(glm::vec2(position.x, position.z), story, Util::StringToAxis(axis), rotateFloor));
			}
		}	
		
		// Staircases
		if (document.HasMember("STAIRCASES"))
		{
			const rapidjson::Value& rooms = document["STAIRCASES"];
			for (rapidjson::SizeType i = 0; i < rooms.Size(); i++)
			{
				glm::vec2 position = ReadVec2(rooms[i], "Position");
				std::string axis = ReadString(rooms[i], "Axis");
				int story = ReadInt(rooms[i], "Story");
				int firstSetCount = ReadInt(rooms[i], "FirstSetCount");
				bool turnsLeft = ReadBool(rooms[i], "TurnsLeft");
				bool FloorRotateBottom = ReadBool(rooms[i], "FloorRotateBottom");
				bool FloorRotateTop = ReadBool(rooms[i], "FloorRotateTop");
				bool FloorRotateLanding = ReadBool(rooms[i], "FloorRotateLanding");

				house.AddStaircase(Staircase(
					position, 
					Util::StringToAxis(axis), 
					story, 
					firstSetCount, 
					turnsLeft, 
					FloorRotateBottom, 
					FloorRotateTop, 
					FloorRotateLanding));
			}
		}

		// Lights
		if (document.HasMember("LIGHTS"))
		{
			const rapidjson::Value& lights = document["LIGHTS"];
			for (rapidjson::SizeType i = 0; i < lights.Size(); i++)
			{
				glm::vec3 position = ReadVec3(lights[i], "Position");
				glm::vec3 modelPosition = ReadVec3(lights[i], "ModelPosition");
				glm::vec3 modelRotation = ReadVec3(lights[i], "ModelRotation");
				glm::vec3 modelScale = ReadVec3(lights[i], "ModelScale");
				glm::vec3 color = ReadVec3(lights[i], "Color");
				float radius = ReadFloat(lights[i], "Radius");
				float magic = ReadFloat(lights[i], "Magic");
				float strength = ReadFloat(lights[i], "Strength");
				
				house.AddLight(Light(position, color, radius, strength, magic, Transform(modelPosition, modelRotation, modelScale)));
			}
		}

		
		if (document.HasMember("ENTITIES"))
		{
			const rapidjson::Value& entities = document["ENTITIES"];
			for (rapidjson::SizeType i = 0; i < entities.Size(); i++)
			{
				glm::vec3 Position = ReadVec3(entities[i], "Position");
				glm::vec3 Rotation = ReadVec3(entities[i], "Rotation");
				glm::vec3 Scale = ReadVec3(entities[i], "Scale");
				char* Tag = ReadText(entities[i], "Tag");
				char* ModelName = ReadText(entities[i], "ModelName");
				char* MaterialName = ReadText(entities[i], "MaterialName");

				Entity e(Tag);
				e.m_transform.position = Position;
				e.m_transform.rotation = Rotation;
				e.m_transform.scale = Scale;
				e.m_modelID = AssetManager::GetModelIDByName(ModelName);
				e.m_materialID = AssetManager::GetMaterialIDByName(MaterialName);
				house.m_entities.push_back(e);
			}
		}

		return house;
	}

	void File::SaveMap(std::string filename, House * house)
	{
		rapidjson::Document document;
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		rapidjson::Value roomsArray(rapidjson::kArrayType);
		rapidjson::Value doorsArray(rapidjson::kArrayType);
		rapidjson::Value lightsArray(rapidjson::kArrayType);
		rapidjson::Value staircasesArray(rapidjson::kArrayType);
		rapidjson::Value entitiesArray(rapidjson::kArrayType);
		document.SetObject();

		// Save Entities
		for (Entity& entity : house->m_entities)
		{
			rapidjson::Value object(rapidjson::kObjectType);
			SaveVec3(&object, "Position", entity.m_transform.position, allocator);
			SaveVec3(&object, "Rotation", entity.m_transform.rotation, allocator);
			SaveVec3(&object, "Scale", entity.m_transform.scale, allocator);
			SaveString(&object, "ModelName", AssetManager::GetModelNameByID(entity.m_modelID), allocator);
			SaveString(&object, "MaterialName", AssetManager::GetMaterialNameByID(entity.m_materialID), allocator);
			SaveString(&object, "Tag", entity.m_tag, allocator);
			entitiesArray.PushBack(object, allocator);
		}			
		
		// Save rooms
		for (Room & room : house->m_rooms)
		{
			rapidjson::Value roomObject(rapidjson::kObjectType);
			SaveVec2(&roomObject, "Position", room.m_position, allocator);
			SaveVec2(&roomObject, "Size", room.m_size, allocator); 
			SaveInt(&roomObject, "Story", room.m_story, allocator);
			roomsArray.PushBack(roomObject, allocator);
		}		

		// Save doors
		for (Door& door : house->m_doors)
		{
			rapidjson::Value object(rapidjson::kObjectType);
			SaveVec3(&object, "Position", door.m_rootTransform.position, allocator);
			SaveInt(&object, "Story", door.m_story, allocator);
			SaveString(&object, "Axis", Util::AxisToString(door.m_axis), allocator);
			SaveBool(&object, "RotateFloorTexture", door.m_floor.m_rotateTexture, allocator);
			doorsArray.PushBack(object, allocator);
		}

		// Save staircases
		for (Staircase& staircase : house->m_staircases)
		{
			rapidjson::Value object(rapidjson::kObjectType);
			SaveVec2(&object, "Position", glm::vec2(staircase.m_rootTransform.position.x, staircase.m_rootTransform.position.z), allocator);
			SaveInt(&object, "Story", staircase.m_story, allocator);
			SaveString(&object, "Axis", Util::AxisToString(staircase.m_axis), allocator);
			SaveInt(&object, "FirstSetCount", staircase.m_stepsInFirstSet, allocator);
			SaveBool(&object, "TurnsLeft", staircase.m_turnsLeft, allocator);
			SaveBool(&object, "FloorRotateBottom", staircase.m_bottomDoorwayFloor.m_rotateTexture, allocator);
			SaveBool(&object, "FloorRotateTop", staircase.m_topDoorwayFloor.m_rotateTexture, allocator);
			SaveBool(&object, "FloorRotateLanding", staircase.m_landingFloor.m_rotateTexture, allocator);
			staircasesArray.PushBack(object, allocator);
		}

		// Save Lights
		for (Light& light : house->m_lights)
		{
			rapidjson::Value object(rapidjson::kObjectType);
			SaveVec3(&object, "Position", light.m_position, allocator);
			SaveVec3(&object, "ModelPosition", light.m_modelTransform.position, allocator);
			SaveVec3(&object, "ModelRotation", light.m_modelTransform.rotation, allocator);
			SaveVec3(&object, "ModelScale", light.m_modelTransform.scale, allocator);
			SaveVec3(&object, "Color", light.m_color, allocator);
			SaveFloat(&object, "Radius", light.m_radius, allocator);
			SaveFloat(&object, "Strength", light.m_strength, allocator);
			SaveFloat(&object, "Magic", light.m_magic, allocator);
			lightsArray.PushBack(object, allocator);
		}

		// Add arrays
		document.AddMember("ENTITIES", entitiesArray, allocator);
		document.AddMember("ROOMS", roomsArray, allocator);
		document.AddMember("DOORS", doorsArray, allocator);
		document.AddMember("LIGHTS", lightsArray, allocator);
		document.AddMember("STAIRCASES", staircasesArray, allocator);

		// Convert JSON document to string
		rapidjson::StringBuffer strbuf;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
		document.Accept(writer);

		// Save it
		std::string data = strbuf.GetString();
		std::ofstream out("res/maps/" + filename);
		out << data;
		out.close();
	}

	void File::SaveVec3(rapidjson::Value* object, std::string elementName, glm::vec3 vector, rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value array(rapidjson::kArrayType);
		rapidjson::Value name(elementName.c_str(), allocator);
		array.PushBack(rapidjson::Value().SetFloat(vector.x), allocator);
		array.PushBack(rapidjson::Value().SetFloat(vector.y), allocator);
		array.PushBack(rapidjson::Value().SetFloat(vector.z), allocator);
		object->AddMember(name, array, allocator);
	}

	void File::SaveVec2(rapidjson::Value* object, std::string elementName, glm::vec2 vector, rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value array(rapidjson::kArrayType);
		rapidjson::Value name(elementName.c_str(), allocator);
		array.PushBack(rapidjson::Value().SetFloat(vector.x), allocator);
		array.PushBack(rapidjson::Value().SetFloat(vector.y), allocator);
		object->AddMember(name, array, allocator);
	}

	void File::SaveString(rapidjson::Value* object, std::string elementName, std::string string, rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value name(elementName.c_str(), allocator);
		rapidjson::Value value(rapidjson::kObjectType);
		value.SetString(string.c_str(), static_cast<rapidjson::SizeType>(string.length()), allocator);
		object->AddMember(name, value, allocator);
	}

	void File::SaveBool(rapidjson::Value* object, std::string elementName, bool boolean, rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value name(elementName.c_str(), allocator);
		rapidjson::Value value(rapidjson::kObjectType);
		value.SetBool(boolean);
		object->AddMember(name, value, allocator);
	}

	void File::SaveFloat(rapidjson::Value* object, std::string elementName, float number, rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value name(elementName.c_str(), allocator);
		rapidjson::Value value(rapidjson::kObjectType);
		value.SetFloat(number);
		object->AddMember(name, value, allocator);
	}

	void File::SaveInt(rapidjson::Value* object, std::string elementName, int number, rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value name(elementName.c_str(), allocator);
		rapidjson::Value value(rapidjson::kObjectType);
		value.SetInt(number);
		object->AddMember(name, value, allocator);
	}


	glm::vec3 File::ReadVec3(const rapidjson::Value& value, std::string name)
	{
		glm::vec3 v = glm::vec3(0, 0, 0);
		if (value.HasMember(name.c_str())) {
			const rapidjson::Value& element = value[name.c_str()];
			v.x = element[0].GetFloat();
			v.y = element[1].GetFloat();
			v.z = element[2].GetFloat();
		}
		else
			std::cout << "'" << name << "' NOT FOUND IN MAP FILE\n";
		return v;
	}

	glm::vec2 File::ReadVec2(const rapidjson::Value& value, std::string name)
	{
		glm::vec2 v = glm::vec2(0, 0);
		if (value.HasMember(name.c_str())) {
			const rapidjson::Value& element = value[name.c_str()];
			v.x = element[0].GetFloat();
			v.y = element[1].GetFloat();
		}
		else
			std::cout << "'" << name << "' NOT FOUND IN MAP FILE\n";
		return v;
	}

	std::string File::ReadString(const rapidjson::Value & value, std::string name)
	{
		std::string s = "NOT FOUND";
		if (value.HasMember(name.c_str()))
			s = value[name.c_str()].GetString();
		else
			std::cout << "'" << name << "' NOT FOUND IN MAP FILE\n";
		return s;
	}

	char* File::ReadText(const rapidjson::Value& value, std::string name)
	{
		std::string s = "NOT FOUND";
		if (value.HasMember(name.c_str()))
			s = value[name.c_str()].GetString();
		else
			std::cout << "'" << name << "' NOT FOUND IN MAP FILE\n";
		
		char* cstr = new char[s.length() + 1];
		strcpy(cstr, s.c_str());
		// do stuff
		//delete[] cstr;		
		return cstr;
	}

	bool File::ReadBool(const rapidjson::Value & value, std::string name)
	{
		bool b = false;
		if (value.HasMember(name.c_str()))
			b = value[name.c_str()].GetBool();
		else
			std::cout << "'" << name << "' NOT FOUND IN MAP FILE\n";
		return b;
	}

	float File::ReadFloat(const rapidjson::Value& value, std::string name)
	{
		float f = -1;

		if (value.HasMember(name.c_str()))
			f = value[name.c_str()].GetFloat();
		else
			std::cout << "'" << name << "' NOT FOUND IN MAP FILE\n";
		return f;
	}

	int File::ReadInt(const rapidjson::Value& value, std::string name)
	{
		float f = -1;

		if (value.HasMember(name.c_str()))
			f = value[name.c_str()].GetInt();
		else
			std::cout << "'" << name << "' NOT FOUND IN MAP FILE\n";
		return f;
	}
}
