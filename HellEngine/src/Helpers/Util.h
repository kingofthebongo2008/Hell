#pragma once
#include "Header.h"
#include "Renderer/Transform.h"
#include "Renderer/Shader.h"

#include "bullet/src/btBulletCollisionCommon.h"
#include "bullet/src/btBulletDynamicsCommon.h"
#include "bullet/src/LinearMath/btVector3.h"
#include "bullet/src/LinearMath/btAlignedObjectArray.h"

#include "assimp/matrix4x4.h"
#include "assimp/matrix3x3.h"

namespace HellEngine
{
	class Util
	{
	public:
		static glm::mat4 Get_Mat4_From_Vec3(glm::vec3 position);
		static void TranslatePosition(glm::vec3* position, glm::mat4 translation);
		static glm::vec3 Position_From_Mat_4(glm::mat4 matrix);
		static float Square(float value);
		static float FInterpTo(float Current, float Target, float DeltaTime, float InterpSpeed);
		static glm::vec3 Vec3InterpTo(glm::vec3 Current, glm::vec3 Target, float DeltaTime, float InterpSpeed); 
		static glm::vec3 ProjectVec3(glm::vec3 vector, const glm::vec3& other); // Projection of this on to other
		static bool StringContainsString(const char* string, const char* query); 
		static std::string RemoveCharactersFromEnd(std::string string, int number);
		static std::string RemoveCharactersFromBeginning(std::string string, int number);
		static bool Util::StringEndsIn(std::string const& fullString, std::string const& ending);		
		static void DrawUpFacingPlane(Shader* shader, Transform* transform);
		static void DrawDownFacingPlane(Shader* shader, Transform* transform);
		static void SetNormalsAndTangentsFromVertices(Vertex* vert0, Vertex* vert1, Vertex* vert2);
		static void SetTangentsFromVertices(Vertex* vert0, Vertex* vert1, Vertex* vert2);
		static glm::vec3 Util::NormalFromTriangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2);
		static glm::vec3 SetRotationByAxis(Axis axis);
		static std::string FileNameFromPath(std::string filepath);
		static FileType FileTypeFromPath(std::string filepath);
		static btVector3 glmVec3_to_btVec3(glm::vec3);
		static glm::vec3 btVec3_to_glmVec3(btVector3);
		static std::string Vec3_to_String(glm::vec3 vector);

		static std::string AxisToString(Axis axis);
		static Axis StringToAxis(std::string string);

		static glm::vec3 GetTranslationFromMatrix(glm::mat4 matrix);
		static void DrawPoint(Shader* shader, glm::vec3 position, glm::mat4 modelMatrix, glm::vec3 color);
		static float Util::RandomFloat(float a, float b);
		static void PrintInRed(const char* text);

		static glm::mat4 EmptyMat4();
		static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);
		static glm::mat4 aiMatrix3x3ToGlm(const aiMatrix3x3& from);
		static glm::mat4 Mat4InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
		static glm::mat4 Mat4InitRotateTransform(float RotateX, float RotateY, float RotateZ);
		static glm::mat4 Mat4InitTranslationTransform(float x, float y, float z);
		static std::string Mat4ToString(glm::mat4 m);
		static void PrintMat4(glm::mat4 m);

	
	private:
			static unsigned int upFacingPlaneVAO;
			static unsigned int downFacingPlaneVAO;
	};
}