/**********************************************************************************
* \file   DeltaEngine.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
//For use by Delta Engine Applications
#include "Application.h"

// Debugging
#include "Core/Debugging/Logger/Log.h"

// Core
#include "Core/GameClock/EngineClock.h"
#include "ECS/ECSModule.h"

// Math
#include "Core/Math/DE_Math.h"

// AssetManager
#include "Assets/AssetManager.h"
#include "Assets/Loaders/FontLoader.h"
#include "Assets/Loaders/ShaderLoader.h"
#include "Assets/Loaders/TextureLoader.h"
#include "Assets/Loaders/AnimationClipLoader.h"
#include "Assets/Loaders/AnimationControllerLoader.h"


#include "Physics/Collision.h"
// Render
#include "Render/Window.h"
#include "Render/ErrorCheck.h"
#include "Render/Mesh.h"
#include "Render/OpenGLSystem.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Material.h"

#include "Event/ApplicationEvent.h"

#include "Core/GlobalStruct.h"
