/**********************************************************************************
* \file   ExitSceneCinematic.h
* \brief  The file contains the code for exiting the scene
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Core/Math/Vector.h"
#include "ECS/ECSModule.h"
#include "Components/Transform.h"
namespace DeltaEngine
{
DEFINE_SYSTEM(ExitSceneCinematic, Transform)
    void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void CreditsStart();
	void LoadMainMenuAgain();

private:
	Vector2 ExitPoint;
	Vector2 StopPoint;
	float timer;
	float credits_timer;
	bool ExitPointTriggered;
	bool StopPointTriggered;
	bool CreditsRolledTriggered;
	bool MainMenuTriggered;
END_DEFINE_SYSTEM(ExitSceneCinematic)
}
