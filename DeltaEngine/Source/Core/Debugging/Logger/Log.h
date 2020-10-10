#pragma once

#include "DE_API.h"
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)
#include "DEpch.h"
#include <fstream>

namespace DeltaEngine
{
	class DE_API Log
	{

	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger()   { return _coreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _clientLogger; }

	private:
#pragma warning(disable:4251)
		static std::shared_ptr<spdlog::logger> _coreLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;
		static std::shared_ptr<spdlog::logger> core_file_logger;
#pragma warning(default:4251)
	};
}


//CORE LOG MACROS
#define DeltaEngine_CORE_TRACE(...) ::DeltaEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DeltaEngine_CORE_INFO(...)  ::DeltaEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DeltaEngine_CORE_WARN(...)  ::DeltaEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DeltaEngine_CORE_ERROR(...) ::DeltaEngine::Log::GetCoreLogger()->error(__VA_ARGS__);spdlog::get("core_file_logger")->error(__VA_ARGS__) 
#define DeltaEngine_CORE_FATAL(...) ::DeltaEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//eg -> //DeltaEngine_CORE_TRACE("Loading shader \"{}\"...", filepath.c_str());


//CLIENT LOG APP MACROS
#define DeltaEngine_TRACE(...)      ::DeltaEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DeltaEngine_INFO(...)       ::DeltaEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define DeltaEngine_WARN(...)       ::DeltaEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DeltaEngine_ERROR(...)      ::DeltaEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define DeltaEngine_FATAL(...)      ::DeltaEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)

#define DeltaEngine_TRACE_TO_FILE(...) spdlog::get("client_file_logger")->trace(__VA_ARGS__);
#define DeltaEngine_INFO_TO_FILE(...) spdlog::get("client_file_logger")->info(__VA_ARGS__);
#define DeltaEngine_WARN_TO_FILE(...) spdlog::get("client_file_logger")->warn(__VA_ARGS__);
#define DeltaEngine_ERROR_TO_FILE(...) spdlog::get("client_file_logger")->error(__VA_ARGS__);
#define DeltaEngine_FATAL_TO_FILE(...) spdlog::get("client_file_logger")->fatal(__VA_ARGS__);

#define DeltaEngine_LOG_TO_FILE(...) spdlog::get("core_file_logger")->info(__VA_ARGS__);
//DeltaEngine_LOG_TO_FILE("TESTING123"); -> will write to DeltaEngine\Sandbox\Logs