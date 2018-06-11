#pragma once
#include <functional>
#include <unordered_map>

#include <spdlog/spdlog.h>
#include "property.h"

namespace general
{
	namespace log_config_key
	{
		const std::string kLoggerName("kLoggerName");
		const std::string kLoggerFilename("kLoggerFilename");
		const std::string kLoggerPattern("kLoggerPattern");
		const std::string kUseConsoleLogger("kUseConsoleLogger");
		const std::string kLoggerThreadMode("kLoggerThreadMode");
		const std::string kLoggerType("kLogType");
		const std::string kBasicTruncate("kBasicTruncate");
		const std::string kRotatingMaxFileSize("kRotatingMaxFileSize");
		const std::string kRotatingMaxFiles("kRotatingMaxFiles");
		const std::string kDailyHour("kDailyHour");
		const std::string kDailyMinute("kDailyMinute");

		enum class LoggerThreadMode :uint8_t
		{
			kLoggerSt,
			kLoggerMt
		};

		enum class LoggerType :uint8_t
		{
			kLoggerTypeBasic,
			kLoggerTypeRotating,
			kLoggerTypeDaily
		};

		namespace default_value
		{
			const std::string kLoggerNameValue("general_log");
			const spdlog::filename_t kLoggerFilenameValue("./log/general_log.log");
			const std::string kLoggerPatternValue("*** [%Y-%m-%d %H:%M:%S,%f] %v ***");
			const bool kUseConsoleLoggerValue(true);
			const uint8_t kLoggerThreadModeValue = static_cast<uint8_t>(LoggerThreadMode::kLoggerSt);
			const uint8_t kLoggerTypeValue = static_cast<uint8_t>(LoggerType::kLoggerTypeBasic);
			const bool kBasicTruncateValue(false);
			const size_t kRotatingMaxFileSizeValue(1024 * 1024 * 1024);
			const size_t kRotatingMaxFilesValue(9);
			const int kDailyHourValue(0);
			const int kDailyMinuteValue(0);
		}

	}

	class Log
	{
	public:
		enum class ErrorCode
		{
			kSuccess,
			kAlreadyInit,
			CreateLoggerDirectoriesFailed,
			kFailure
		};

	public:
		int32_t Init(const Property& prop);
	private:
		static bool s_use_console_logger;
		static std::shared_ptr<spdlog::logger> s_logger;
		static std::shared_ptr<spdlog::logger> s_console;
	};

}
#define LOG_TRECE(...);
#define LOG_DEBUG(...);
#define LOG_INFO(...);
#define LOG_WARN(...);
#define LOG_ERROR(...);
#define LOG_CRITICAL(...);
