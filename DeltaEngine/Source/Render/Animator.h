#pragma once

#include "AnimationClip.h"
#include "AnimationController.h"
#include "SpriteRenderer.h"

namespace DeltaEngine
{
	struct Animator
	{
	private:
		using Parameters = AnimationController::Parameters;
		Parameters parameters;

		float m_Timer;
		unsigned int m_Frame;
		float m_Speed;
	public:
		SpriteRenderer* renderer;
		AnimationController* m_Controller;
		AnimationClip* m_Clip;

		Animator(AnimationController* controller = nullptr);

		bool GetBool(std::string paramName);
		bool SetBool(std::string paramName, bool value);

		float GetFloat(std::string paramName);
		float SetFloat(std::string paramName, float value);
		
		unsigned int GetFrame() const;

		void Update();
	private:
		void CheckCondition();
	};
}
