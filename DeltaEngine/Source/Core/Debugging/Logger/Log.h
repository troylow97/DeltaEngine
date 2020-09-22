#pragma once

#include "DE_API.h"
#include "spdlog/spdlog.h"

namespace DeltaEngine
{
	class DE_API Log
	{

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()   { return _coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> _coreLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;

	};
}

//CORE LOG MACROS
#define DeltaEngine_CORE_TRACE(...) ::DeltaEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DeltaEngine_CORE_INFO(...)  ::DeltaEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DeltaEngine_CORE_WARN(...)  ::DeltaEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DeltaEngine_CORE_ERROR(...) ::DeltaEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DeltaEngine_CORE_FATAL(...) ::DeltaEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//CLIENT LOG MACROS
#define DeltaEngine_TRACE(...)      ::DeltaEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DeltaEngine_INFO(...)       ::DeltaEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define DeltaEngine_WARN(...)       ::DeltaEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DeltaEngine_ERROR(...)      ::DeltaEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define DeltaEngine_FATAL(...)      ::DeltaEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)