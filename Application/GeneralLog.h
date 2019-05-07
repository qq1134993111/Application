#pragma once
#include <functional>
#include <unordered_map>

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
		const std::string kLoggerType("kLogType");
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
			kLoggerSt=0,
			kLoggerMt=2
		};

		enum class LoggerType :uint8_t
		{
			kLoggerTypeBasic,
			kLoggerTypeRotating,
			kLoggerTypeDaily
		};

		namespace default_value
		{

			const spdlog::filename_t kLoggerFilenameValue("./log/general_log.log");
			const std::string kLoggerPatternValue("[%Y-%m-%d %H:%M:%S.%f] [%P,%t] [%l]  %v [%!,%@]");
			const bool kUseConsoleLoggerValue(true);
			const uint8_t kLoggerThreadModeValue = static_cast<uint8_t>(LoggerThreadMode::kLoggerSt);
			const uint8_t kLoggerTypeValue = static_cast<uint8_t>(LoggerType::kLoggerTypeBasic);
			const bool kBasicTruncateValue(false);
			const size_t kRotatingMaxFileSizeValue(1024 * 1024 * 1024);
			const size_t kRotatingMaxFilesValue(9);
			const int kDailyHourValue(0);
			const int kDailyMinuteValue(0);
			const bool kAsyncModeValue(false);
			const uint32_t kAsyncQueueSizeValue(8092);
			const std::string kLoggerLevelValue("trace");
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
			//s_logger.reset();
			//s_console.reset();	
		}

		static inline void Shutdown()
		{
			spdlog::shutdown();
		}

		template<typename... Args>
		static void Log(spdlog::level::level_enum lvl, const char *fmt, Args &&... args)
		{
			if (s_console)
			{
				s_console->log(lvl, fmt, std::forward<Args>(args)...);
			}
			if (s_logger)
			{
				s_logger->log(lvl, fmt, std::forward<Args>(args)...);
			}
		}

		

		template<typename... Args>
		static void Trace(const char *fmt, Args &&... args)
		{
			if (s_console)
			{
				s_console->trace(fmt, std::forward<Args>(args)...);
			}
			if (s_logger)
			{
				s_logger->trace(fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void Debug(const char *fmt, Args &&... args)
		{
			if (s_console)
			{
				s_console->debug(fmt, std::forward<Args>(args)...);
			}
			if (s_logger)
			{
				s_logger->debug(fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void Info(const char *fmt, Args &&... args)
		{
			if (s_console)
			{
				s_console->info(fmt, std::forward<Args>(args)...);
			}
			if (s_logger)
			{
				s_logger->info(fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void Warn(const char *fmt, Args &&... args)
		{
			if (s_console)
			{
				s_console->warn(fmt, std::forward<Args>(args)...);
			}
			if (s_logger)
			{
				s_logger->warn(fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void Error(const char *fmt, Args &&... args)
		{
			if (s_console)
			{
				s_console->error(fmt, std::forward<Args>(args)...);
			}
			if (s_logger)
			{
				s_logger->error(fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void Critical(const char *fmt, Args &&... args)
		{
			if (s_console)
			{
				s_console->critical(fmt, std::forward<Args>(args)...);
			}
			if (s_logger)
			{
				s_logger->critical(fmt, std::forward<Args>(args)...);
			}
		}

	private:
		static std::shared_ptr<spdlog::logger> s_logger;
		static std::shared_ptr<spdlog::logger> s_console;
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
//#define GENERAL_LOG_STR_H(x) #x
//#define GENERAL_LOG_STR_HELPER(x) GENERAL_LOG_STR_H(x)
//#define LOG_TRACE(...)          general::GeneralLog::Trace(__VA_ARGS__" ["  "][" __FILE__ "(" GENERAL_LOG_STR_HELPER(__LINE__) ")]" )
//#define LOG_DEBUG(...)          general::GeneralLog::Debug(__VA_ARGS__" ["  "][" __FILE__ "(" GENERAL_LOG_STR_HELPER(__LINE__) ")]" )
#define LOG_TRACE(...)            general::GeneralLog::Trace(__VA_ARGS__)
#define LOG_DEBUG(...)            general::GeneralLog::Debug(__VA_ARGS__)
#define LOG_INFO(...)             general::GeneralLog::Info(__VA_ARGS__)
#define LOG_WARN(...)             general::GeneralLog::Warn(__VA_ARGS__)
#define LOG_ERROR(...)            general::GeneralLog::Error(__VA_ARGS__)
#define LOG_CRITICAL(...)         general::GeneralLog::Critical(__VA_ARGS__)

