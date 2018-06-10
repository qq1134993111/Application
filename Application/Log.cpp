#include "Log.h"


bool Log::s_use_console_logger=false;
std::shared_ptr<spdlog::logger> Log::s_logger=nullptr;
std::shared_ptr<spdlog::logger> Log::s_console = nullptr;
