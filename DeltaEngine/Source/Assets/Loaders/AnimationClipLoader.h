#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/AnimationClip.h"

namespace DeltaEngine
{
    class AnimationClipLoader final : public AbstractLoader<AnimationClip>
    {
        void DoLoad(AssetKey key) override
        {
        }

        void DoLoad(AssetKey key, std::string_view str) override
        {
            AnimationClip* data = new AnimationClip{ std::string(str) };
            // To define how to handle failed loading
            //if (data.characterInfo().empty())
            //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
            //else
            Set(key, data, AssetState::Final, AssetLifetime::Persistent);
        }
    };
} // namespace DeltaEngine
