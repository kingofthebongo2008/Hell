#pragma once
#include "Header.h"

namespace HellEngine
{
	class LightVolume
	{
	public: // methods
		LightVolume();
		LightVolume(float lowerX, float lowerZ, float upperX, float upperZ);
	//	LightVolume(DoorHole* doorHole, Light light);
	//	~LightVolume();
		void Draw(Shader* shader);

		void Setup();

	public: // fields
		unsigned int VAO;
		std::vector<glm::vec3> vertices;

	public: // static
		//static LightVolume BuildLightVolumeFromDoorHole(DoorHole doorHole, Light light, float floorHeight, float bias);
	};
}