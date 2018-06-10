#pragma once
#include <spdlog/spdlog.h>
#include "property.h"


class Log
{
public:

	enum class ErrorCode
	{
		kSuccess,
		kAlreadyInit,
		kFailure
	};

public:
	int32_t Init(const Property& prop)
	{
		if (s_logger==nullptr)
		{
			//s_logger = spdlog::rotating_logger_mt();
		}

		return  static_cast<int32_t>(ErrorCode::kAlreadyInit);
	}
private:
	static bool s_use_console_logger;
	static std::shared_ptr<spdlog::logger> s_logger;
	static std::shared_ptr<spdlog::logger> s_console;
};

#define LOG_TRECE(...);
#define LOG_DEBUG(...);
#define LOG_INFO(...);
#define LOG_WARN(...);
#define LOG_ERROR(...);
#define LOG_CRITICAL(...);
