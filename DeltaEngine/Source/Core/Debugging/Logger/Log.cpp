/**********************************************************************************
* \file   Log.cpp
* \brief  The file contains a logger class for debugging info
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
    //FileUtils::CreateDir(LOG_DIR);
    //FileUtils::CreateFile(LOG_TEXT_NAME);

    spdlog::set_pattern("%^[%T] %n: %v%$");
    _coreLogger = spdlog::stdout_color_mt("DELTAENGINE");

#ifdef DE_EDITOR
    _coreLogger->set_level(spdlog::level::trace);
#else
    _coreLogger->set_level(spdlog::level::off);
#endif


    _clientLogger = spdlog::stdout_color_mt("APP");
#ifdef DE_EDITOR
    _coreLogger->set_level(spdlog::level::trace);
#else
    _coreLogger->set_level(spdlog::level::off);
#endif
    //core_file_logger = spdlog::basic_logger_mt("core_file_logger", LOG_TEXT_NAME);

    spdlog::flush_on(spdlog::level::info);
  }
}
