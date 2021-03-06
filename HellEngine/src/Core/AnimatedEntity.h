#pragma once
#include "Header.h"
#include "Helpers/AssetManager.h"

namespace HellEngine
{
	class AnimatedEntity
	{
	public: // methods
		AnimatedEntity();
		void Update(float deltatime);
		//void Draw(Shader* shader);
		void Draw(Shader* shader, glm::mat4 modelMatrix);
		void SetSkinnedModel(const char* skinnedModelName);
		void PlayAnimation(const char* animationName, bool loop);
		glm::mat4 GetCameraMatrix();
		bool IsAnimationComplete();
		bool IsSpecificAnimationComplete(const char* animationName);
		void ResetAnimationTimer();
		void PauseOnFinalFrame();
		void SetModelScale(float scale);
		void FlipModelUpAxis(bool flip);
		void FlipSkeletonUpAxis(bool flip);
		void SetAnimationToBindPose();

	public:	// fields
		Transform m_worldTransform;
		Transform m_modelTransform;
		Transform m_skeletonTransform;
		int m_skinnedModelID = -1;
		std::vector<int> m_meshMaterialIDs;
		std::vector<glm::mat4> m_animatedTransforms;
		std::vector<glm::mat4> m_animatedDebugTransforms_Animated;
		//std::vector<glm::mat4> m_animatedDebugTransforms_BindPose;
		int m_currentAnimationIndex;
		float m_currentAnimationTime;
		float m_currentAnimationDuration;

	private: // methods
		void UpdateAnimation(float deltatime);

	private: // fields
		bool m_loopCurrentAnimation;
		float m_animationSpeed;
		//Transform m_worldTransform;
		bool m_animationIsComplete;
		
	};
}