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
		const std::string kAsyncMode("kAsyncMode");
		const std::string kAsyncQueueSize("kAsyncQueueSize");
		const std::string kLoggerLevel("kLoggerLevel");

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
			const bool kAsyncModeValue(false);
			const uint32_t kAsyncQueueSizeValue(8092);
			const std::string kLoggerLevelValue("trace");
		}

	}



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

		static inline void FlushOn(spdlog::level::level_enum log_level)
		{
			spdlog::flush_on(log_level);
		}

		static inline void SetSyncMode()
		{
			spdlog::set_sync_mode();
		}

		static inline void SetAsyncMode(size_t queue_size, const spdlog::async_overflow_policy overflow_policy = spdlog::async_overflow_policy::block_retry,
			const std::function<void()> &worker_warmup_cb = nullptr,
			const std::chrono::milliseconds &flush_interval_ms = std::chrono::milliseconds::zero(),
			const std::function<void()> &worker_teardown_cb = nullptr)
		{
			spdlog::set_async_mode(queue_size, overflow_policy, worker_warmup_cb, flush_interval_ms, worker_teardown_cb);
		}

		static inline void SetErrorHandler(spdlog::log_err_handler handler)
		{
			spdlog::set_error_handler(handler);
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

#define LOG_INIT(prop) general::GeneralLog::Init(prop)
#define LOG_SET_LEVEL(l) general::GeneralLog::SetLevel(l)
#define LOG_FLUSH_ON(l)  general::GeneralLog::FlushOn(l)
#define LOG_SET_SYNC_MODE()  general::GeneralLog::SetSyncMode()
#define LOG_SET_ASYNC_MODE(queue_size,overflow_policy,\
worker_warmup_cb,flush_interval_ms,worker_teardown_cb)\
general::GeneralLog::SetASyncMode(queue_size,overflow_policy,worker_warmup_cb,flush_interval_ms,worker_teardown_cb)
#define LOG_SET_ERROR_HANDLE(cb) general::GeneralLog::SetErrorHandler(cb)
#define LOG_TRECE(...) general::GeneralLog::Trace(__VA_ARGS__"[ " __FUNCTION__  "][" __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") ] ")
#define LOG_DEBUG(...) general::GeneralLog::Debug(__VA_ARGS__"[ " __FUNCTION__  "][" __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") ] ")
#define LOG_INFO(...) general::GeneralLog::Info(__VA_ARGS__)
#define LOG_WARN(...) general::GeneralLog::Warn(__VA_ARGS__)
#define LOG_ERROR(...) general::GeneralLog::Error(__VA_ARGS__)
#define LOG_CRITICAL(...) general::GeneralLog::Critical(__VA_ARGS__)