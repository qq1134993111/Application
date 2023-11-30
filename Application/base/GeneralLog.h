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
        const std::string kAsyncThreadCount("kAsyncThreadCount");
        const std::string kAsyncFlushEveryMicrosecondInterval("kAsyncFlushEveryMicrosecondInterval");
        const std::string kBacktraceSize("kBacktraceSize");
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
			const std::string kLoggerPatternValue("[%Y-%m-%d %H:%M:%S.%f] [%-6P,%6t] [%=8l]  %v [%!,%s:%#]");
			const bool kUseConsoleLoggerValue(false);
            const LoggerThreadMode kLoggerThreadModeValue = LoggerThreadMode::kLoggerMt;
            const LoggerType kLoggerTypeValue = LoggerType::kLoggerTypeDaily;
			const bool kBasicTruncateValue(false);
			const size_t kRotatingMaxFileSizeValue(1024 * 1024 * 1024);
			const size_t kRotatingMaxFilesValue(9);
			const int kDailyHourValue(0);
			const int kDailyMinuteValue(0);
			const bool kAsyncModeValue(false);
			const uint32_t kAsyncQueueSizeValue(8092);
            const uint32_t kAsyncThreadCountValue(1);
            const uint32_t kAsyncFlushEveryMicrosecondIntervalValue(100000); 
			const uint32_t kBacktraceSizeValue(0);
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
        using source_loc = spdlog::source_loc;
        using level_enum = spdlog::level::level_enum;
	public:
		static int32_t Init(const Property& prop);

		static inline void SetLevel(level_enum log_level)
		{
			//spdlog::set_level(log_level);
			if (s_logger)
			{
                s_logger->set_level(log_level);
			}
		}

		static inline void SetPattern(const std::string& pattern)
		{
			//spdlog::set_pattern(pattern);
            if (s_logger)
            {
                s_logger->set_pattern(pattern);
            }
		}

		static inline void FlushOn(level_enum log_level = spdlog::level::trace)
		{
			//spdlog::flush_on(log_level);
            if (s_logger)
            {
                s_logger->flush_on(log_level);
            }
		}

		using spdlog_error_handler=void (const std::string &msg);
        static inline void SetErrorHandler(spdlog_error_handler handler)
		{
			//spdlog::set_error_handler(handler);
            if (s_logger)
            {
                s_logger->set_error_handler(handler);
            }
		}

		static inline void EnableBacktrace(size_t n_messages)
		{
            //spdlog::enable_backtrace(n_messages);
            if (s_logger)
            {
                s_logger->enable_backtrace(n_messages);
            }
		}
        static inline void DisableBacktrace()
        {
            //spdlog::disable_backtrace();
            if (s_logger)
            {
                s_logger->disable_backtrace();
            }
        }
		static inline void DumpBacktrace()
        {
            //spdlog::dump_backtrace();
            if (s_logger)
            {
                s_logger->dump_backtrace();
            }
        }

		static inline void DropAll()
        {
            spdlog::drop_all();
        }

        static inline void Shutdown()
        {
            spdlog::shutdown();
        }

        template <typename Rep, typename Period>
        static inline void FlushEvery(std::chrono::duration<Rep, Period> interval)
        {
            spdlog::flush_every(interval);
        }


		template<typename... Args>
		static void Log(source_loc loc,level_enum lvl, const char* fmt, Args&& ... args)
		{
			if (s_logger_raw)
			{
                s_logger_raw->log(loc, lvl, fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void Trace(source_loc loc, const char* fmt, Args&& ... args)
		{
            Log(loc, level_enum::trace, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Debug(source_loc loc, const char* fmt, Args&& ... args)
		{
            Log(loc, level_enum::debug, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Info(source_loc loc, const char* fmt, Args&& ... args)
		{
            Log(loc, level_enum::info, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Warn(spdlog::source_loc loc, const char* fmt, Args&& ... args)
		{
            Log(loc, level_enum::warn, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Error(source_loc loc, const char* fmt, Args&& ... args)
		{
            Log(loc, level_enum::err, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Critical(source_loc loc, const char* fmt, Args&& ... args)
		{
            Log(loc, level_enum::critical, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Log(source_loc loc,level_enum lvl, const wchar_t* fmt, Args&& ... args)
		{
			if (s_logger)
			{
				s_logger->log(loc, lvl, fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void Trace(source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
            Log(loc, level_enum::trace, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Debug(source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
            Log(loc, level_enum::debug, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Info(source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
            Log(loc, level_enum::info, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Warn(source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
            Log(loc, level_enum::warn, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Error(source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
            Log(loc, level_enum::err, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Critical(source_loc loc, const wchar_t* fmt, Args&& ... args)
		{
            Log(loc, level_enum::critical, fmt, std::forward<Args>(args)...);
		}

	private:
		static std::shared_ptr<spdlog::logger> s_logger;
        static spdlog::logger *s_logger_raw ;
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
#define LOG_FLUSH_EVERY(interval) general::GeneralLog::FlushEvery(interval)


#define LOG_LOG(lvl, ...)        general::GeneralLog::Log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},lvl,__VA_ARGS__)


#define LOG_LEVEL_TRACE SPDLOG_LEVEL_TRACE
#define LOG_LEVEL_DEBUG SPDLOG_LEVEL_DEBUG
#define LOG_LEVEL_INFO SPDLOG_LEVEL_INFO
#define LOG_LEVEL_WARN SPDLOG_LEVEL_WARN
#define LOG_LEVEL_ERROR SPDLOG_LEVEL_ERROR
#define LOG_LEVEL_CRITICAL SPDLOG_LEVEL_CRITICAL
#define LOG_LEVEL_OFF SPDLOG_LEVEL_OFF 

//在包含GeneralLog.h前定义LOG_ACTIVE_LEVEL的值编译期过滤掉低级别日志

#if !defined(LOG_ACTIVE_LEVEL)
#define LOG_ACTIVE_LEVEL LOG_LEVEL_TRACE
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_TRACE
#define LOG_TRACE(...) general::GeneralLog::Trace(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, __VA_ARGS__)
#else
#define LOG_TRACE(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(...) general::GeneralLog::Debug(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, __VA_ARGS__)
#else
#define LOG_DEBUG(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(...) general::GeneralLog::Info(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, __VA_ARGS__)
#else
#define LOG_INFO(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_WARN
#define LOG_WARN(...) general::GeneralLog::Warn(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, __VA_ARGS__)
#else
#define LOG_WARN(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(...) general::GeneralLog::Error(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, __VA_ARGS__)
#else
#define LOG_ERROR(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_CRITICAL
#define LOG_CRITICAL(...) general::GeneralLog::Critical(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, __VA_ARGS__)
#else
#define LOG_CRITICAL(...) (void)0
#endif

