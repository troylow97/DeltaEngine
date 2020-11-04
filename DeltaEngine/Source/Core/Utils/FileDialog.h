#pragma once
#include <optional>
#include <string>


namespace DeltaEngine::FileDialogs
{

std::optional<std::string> OpenFile( const char *filter );
std::optional<std::string> SaveFile( const char *filter );

} // namespace DeltaEngine::FileUtils