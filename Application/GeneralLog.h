#pragma once
#include <functional>
#include <unordered_map>
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include <spdlog/spdlog.h>
#include "Property.h"

namespace general
{
	namespace log_config_key
	{
		const std::string kLoggerName("kLoggerName");
		const std::string kLoggerFilename("kLoggerFilename");
		const std::string kLoggerPattern("kLoggerPattern");
		const std::string kUseConsoleLogger("kUseConsoleLogger");
		const std::string kLoggerThreadMode("kLoggerThreadMode");
		const std::string kLoggerType("kLoggerType");
		const std::string kBasicTruncate("kBasicTruncate");
		const std::string kRotatingMaxFileSize("kRotatingMaxFileSize");
		const std::string kRotatingMaxFiles("kRotatingMaxFiles");
		const std::string kDailyHour("kDailyHour");
		const std::string kDailyMinute("kDailyMinute");
		const std::string kAsyncMode("kAsyncMode");
		const std::string kAsyncQueueSize("kAsyncQueueSize");
		const std::string kLoggerLevel("kLoggerLevel");

		enum class LoggerThreadMode :uint8_t
		{
			kLoggerSt = 1,
			kLoggerMt
		};

		enum class LoggerType :uint8_t
		{
			kLoggerTypeConsole,
			kLoggerTypeBasic = 1,
			kLoggerTypeRotating,
			kLoggerTypeDaily
		};

		namespace default_value
		{
			const spdlog::filename_t kLoggerFilenameValue("./log/general_log.log");
			const std::string kLoggerPatternValue("[%Y-%m-%d %H:%M:%S.%f] [%-6P,%6t] [%=8l]  %v [%!,%@]");
			const bool kUseConsoleLoggerValue(false);
			const LoggerThreadMode kLoggerThreadModeValue = LoggerThreadMode::kLoggerSt;
			const LoggerType kLoggerTypeValue = LoggerType::kLoggerTypeBasic;
			const bool kBasicTruncateValue(false);
			const size_t kRotatingMaxFileSizeValue(1024 * 1024 * 1024);
			const size_t kRotatingMaxFilesValue(9);
			const int kDailyHourValue(0);
			const int kDailyMinuteValue(0);
			const bool kAsyncModeValue(false);
			const uint32_t kAsyncQueueSizeValue(8092);
			const std::string kLoggerLevelValue("trace");//  "trace", "debug", "info", "warning", "error", "critical", "off"     
		}

	}

	//日志
	class GeneralLog
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
		static int32_t Init(const Property& prop);

		static inline void SetLevel(spdlog::level::level_enum log_level)
		{
			spdlog::set_level(log_level);
		}

		static inline void SetPattern(const std::string& pattern)
		{
			spdlog::set_pattern(pattern);
		}

		static inline void FlushOn(spdlog::level::level_enum log_level = spdlog::level::trace)
		{
			spdlog::flush_on(log_level);

		}

		static inline void SetErrorHandler(spdlog::log_err_handler handler)
		{
			spdlog::set_error_handler(handler);
		}

		static inline void DropAll()
		{
			spdlog::drop_all();
		}

		static inline void Shutdown()
		{
			spdlog::shutdown();
		}

		template<typename... Args>
		static void Log(spdlog::source_loc loc, spdlog::level::level_enum lvl, const char* fmt, Args&& ... args)
		{
			if (s_logger)
			{
				s_logger->log(loc, lvl, fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void Trace(spdlog::source_loc loc, const char* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::trace, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Debug(spdlog::source_loc loc, const char* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::debug, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Info(spdlog::source_loc loc, const char* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::info, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Warn(spdlog::source_loc loc, const char* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::warn, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Error(spdlog::source_loc loc, const char* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::err, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Critical(spdlog::source_loc loc, const char* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::critical, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Log(spdlog::source_loc loc, spdlog::level::level_enum lvl, const wchar_t* fmt, Args&& ... args)
		{
			if (s_logger)
			{
				s_logger->log(loc, lvl, fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void Trace(spdlog::source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::trace, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Debug(spdlog::source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::debug, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Info(spdlog::source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::info, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Warn(spdlog::source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::warn, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Error(spdlog::source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::err, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Critical(spdlog::source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
			Log(loc, spdlog::level::critical, fmt, std::forward<Args>(args)...);
		}

	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};

}



#define LOG_INIT(prop)           general::GeneralLog::Init(prop)
#define LOG_SET_LEVEL(l)         general::GeneralLog::SetLevel(l)
#define LOG_SET_PATTERN(s)       general::GeneralLog::SetPattern(s);
#define LOG_FLUSH_ON(l)          general::GeneralLog::FlushOn(l)
#define LOG_FLUSH_ON_ALL()       general::GeneralLog::FlushOn()
#define LOG_SET_ERROR_HANDLE(cb) general::GeneralLog::SetErrorHandler(cb)
#define LOG_DROP_ALL()           general::GeneralLog::DropAll()
#define LOG_SHUTDOWN()           general::GeneralLog::Shutdown()

#define LOG_TRACE(...)           general::GeneralLog::Trace(spdlog::source_loc{SPDLOG_FILE_BASENAME(__FILE__), __LINE__, SPDLOG_FUNCTION},__VA_ARGS__)
#define LOG_DEBUG(...)           general::GeneralLog::Debug(spdlog::source_loc{SPDLOG_FILE_BASENAME(__FILE__), __LINE__, SPDLOG_FUNCTION},__VA_ARGS__)
#define LOG_INFO(...)            general::GeneralLog::Info(spdlog::source_loc{SPDLOG_FILE_BASENAME(__FILE__), __LINE__, SPDLOG_FUNCTION},__VA_ARGS__)
#define LOG_WARN(...)            general::GeneralLog::Warn(spdlog::source_loc{SPDLOG_FILE_BASENAME(__FILE__), __LINE__, SPDLOG_FUNCTION},__VA_ARGS__)
#define LOG_ERROR(...)           general::GeneralLog::Error(spdlog::source_loc{SPDLOG_FILE_BASENAME(__FILE__), __LINE__, SPDLOG_FUNCTION},__VA_ARGS__)
#define LOG_CRITICAL(...)        general::GeneralLog::Critical(spdlog::source_loc{SPDLOG_FILE_BASENAME(__FILE__), __LINE__, SPDLOG_FUNCTION},__VA_ARGS__)

