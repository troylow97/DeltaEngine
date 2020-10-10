#include "DEpch.h"
#include "Log.h"
#include "Core/Utils/FileUtils.h"
#define LOG_DIR "Logs"
#define LOG_TEXT_NAME "Logs/basic-log.txt"

namespace DeltaEngine
{

std::shared_ptr<spdlog::logger> Log::_coreLogger;
std::shared_ptr<spdlog::logger> Log::_clientLogger;
std::shared_ptr<spdlog::logger> Log::core_file_logger;

void Log::Init()
{
  FileUtils::CreateDir( LOG_DIR );
  FileUtils::CreateFile( LOG_TEXT_NAME );

  spdlog::set_pattern( "%^[%T] %n: %v%$" );
  _coreLogger = spdlog::stdout_color_mt( "DELTAENGINE" );
  _coreLogger->set_level( spdlog::level::trace );

  _clientLogger = spdlog::stdout_color_mt( "APP" );
  _clientLogger->set_level( spdlog::level::trace );

  core_file_logger = spdlog::basic_logger_mt( "core_file_logger", LOG_TEXT_NAME );

  spdlog::flush_on( spdlog::level::info );


}
}
