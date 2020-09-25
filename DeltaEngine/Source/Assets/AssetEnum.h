#pragma once

namespace DeltaEngine
{
  enum class AssetState : unsigned
  {
    NotLoaded,
    NotLoadedFallback,
    Loading,
    LoadingFallback,
    NotFound,
    NotFoundFallback,
    Mutable,
    Final
  };

  enum class AssetLifetime : unsigned
  {
    Persistant,
    ReferenceCounted,
    Managed
  };
}