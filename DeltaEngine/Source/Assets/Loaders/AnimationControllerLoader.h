#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/AnimationController.h"

namespace DeltaEngine
{
    class AnimationControllerLoader final : public AbstractLoader<AnimationController>
    {
        void DoLoad(AssetKey key) override
        {
        }

        void DoLoad(AssetKey key, std::string_view str) override
        {
            AnimationController* data = new AnimationController{ std::string(str) };
            // To define how to handle failed loading
            //if (data.characterInfo().empty())
            //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
            //else
            Set(key, data, AssetState::Final, AssetLifetime::Persistent);
        }
    };
} // namespace DeltaEngine
