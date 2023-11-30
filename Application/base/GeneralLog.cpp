#include "GeneralLog.h"
#include <boost/filesystem.hpp>
#include <vector>
#include<spdlog/sinks/basic_file_sink.h>
#include<spdlog/sinks/rotating_file_sink.h>
#include<spdlog/sinks/daily_file_sink.h>
#include<spdlog/sinks/stdout_color_sinks.h>
#include<spdlog/async.h>

namespace general
{
	std::shared_ptr<spdlog::logger> GeneralLog::s_logger = nullptr;
    spdlog::logger *GeneralLog::s_logger_raw=nullptr;

    int32_t GeneralLog::Init(const Property &prop)
	{
		if (s_logger != nullptr)
			return  static_cast<int32_t>(ErrorCode::kAlreadyInit);

		auto logger_file_name = prop.GetValue(log_config_key::kLoggerFilename, log_config_key::default_value::kLoggerFilenameValue);

		boost::filesystem::path path(logger_file_name);
		if (!boost::filesystem::exists(path.parent_path()))
		{
			//boost::system::error_code ec;
			if (!boost::filesystem::create_directories(path.parent_path()))
			{
				return static_cast<int32_t>(ErrorCode::CreateLoggerDirectoriesFailed);
			}
		}

		auto logger_name = prop.GetValue(log_config_key::kLoggerName, path.stem().string());

		std::unordered_map<log_config_key::LoggerType, std::unordered_map<log_config_key::LoggerThreadMode, std::function<spdlog::sink_ptr()>>> sink_map
		{
			{
				log_config_key::LoggerType::kLoggerTypeConsole,
				{
				   {
					   log_config_key::LoggerThreadMode::kLoggerSt,
					   [&]()->spdlog::sink_ptr
					   {
						  return std::make_shared<spdlog::sinks::stdout_color_sink_st>();
					   }
					},
					{
					   log_config_key::LoggerThreadMode::kLoggerMt,
					   [&]()->spdlog::sink_ptr
					   {
						  return std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
					   }
					}
			  }
			},
			{
				log_config_key::LoggerType::kLoggerTypeBasic,
				{
				   {
					   log_config_key::LoggerThreadMode::kLoggerSt,
					   [&]()->spdlog::sink_ptr
					   {
						  return std::make_shared<spdlog::sinks::basic_file_sink_st>(
						   logger_file_name,
						   prop.GetValue(log_config_key::kBasicTruncate, log_config_key::default_value::kBasicTruncateValue
						   ));
					   }
					},
					{
					   log_config_key::LoggerThreadMode::kLoggerMt,
					   [&]()->spdlog::sink_ptr
					   {
						  return std::make_shared<spdlog::sinks::basic_file_sink_mt>(
								logger_file_name,
								prop.GetValue(log_config_key::kBasicTruncate, log_config_key::default_value::kBasicTruncateValue)
								);
					   }
					}
			  }
			},
			{
				log_config_key::LoggerType::kLoggerTypeRotating,
				{
					{
					   log_config_key::LoggerThreadMode::kLoggerSt,
					   [&]()->spdlog::sink_ptr
					   {
						  return std::make_shared<spdlog::sinks::rotating_file_sink_st>(
						   logger_file_name,
							prop.GetValue(log_config_key::kRotatingMaxFileSize, log_config_key::default_value::kRotatingMaxFileSizeValue),
							prop.GetValue(log_config_key::kRotatingMaxFiles, log_config_key::default_value::kRotatingMaxFilesValue)
						   );
					   }
					},
					{
					   log_config_key::LoggerThreadMode::kLoggerMt,
					   [&]()->spdlog::sink_ptr
					   {
						   return std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
						   logger_file_name,
						   prop.GetValue(log_config_key::kRotatingMaxFileSize, log_config_key::default_value::kRotatingMaxFileSizeValue),
						   prop.GetValue(log_config_key::kRotatingMaxFiles, log_config_key::default_value::kRotatingMaxFilesValue)
						   );
					   }
					}
				}
			},
			{
				log_config_key::LoggerType::kLoggerTypeDaily,
				{
					{
					   log_config_key::LoggerThreadMode::kLoggerSt,
					   [&]()->spdlog::sink_ptr
					   {
						  return std::make_shared<spdlog::sinks::daily_file_sink_st>(
						   logger_file_name,
						   prop.GetValue(log_config_key::kDailyHour, log_config_key::default_value::kDailyHourValue),
						   prop.GetValue(log_config_key::kDailyMinute, log_config_key::default_value::kDailyMinuteValue),
						   prop.GetValue(log_config_key::kBasicTruncate, log_config_key::default_value::kBasicTruncateValue)
						   );
					   }
					},
					{
						log_config_key::LoggerThreadMode::kLoggerMt,
						[&]()->spdlog::sink_ptr
						{
						   return std::make_shared<spdlog::sinks::daily_file_sink_mt>(
						   logger_file_name,
						   prop.GetValue(log_config_key::kDailyHour, log_config_key::default_value::kDailyHourValue),
						   prop.GetValue(log_config_key::kDailyMinute, log_config_key::default_value::kDailyMinuteValue),
						   prop.GetValue(log_config_key::kBasicTruncate, log_config_key::default_value::kBasicTruncateValue)
						   );
						}
					}
				}
			}
		};

		auto logger_thread_mode = prop.GetValue(log_config_key::kLoggerThreadMode, log_config_key::default_value::kLoggerThreadModeValue);
		auto logger_type = prop.GetValue(log_config_key::kLoggerType, log_config_key::default_value::kLoggerTypeValue);

		auto sink = sink_map[logger_type][logger_thread_mode]();
		std::vector<spdlog::sink_ptr> v_sink{ sink };


		if (prop.GetValue(log_config_key::kUseConsoleLogger, log_config_key::default_value::kUseConsoleLoggerValue))
		{
			auto console_sink = sink_map[log_config_key::LoggerType::kLoggerTypeConsole][logger_thread_mode]();
			v_sink.push_back(console_sink);
		}

		int8_t is_async_mode = prop.GetValue(log_config_key::kAsyncMode, log_config_key::default_value::kAsyncModeValue);
		if (is_async_mode)
		{
			spdlog::init_thread_pool(
				prop.GetValue(log_config_key::kAsyncQueueSize, log_config_key::default_value::kAsyncQueueSizeValue),
				prop.GetValue(log_config_key::kAsyncThreadCount,log_config_key::default_value::kAsyncThreadCountValue));

			s_logger = std::make_shared<spdlog::async_logger>(logger_name, std::begin(v_sink), std::end(v_sink), spdlog::thread_pool(), 
				spdlog::async_overflow_policy::block);
            
			spdlog::flush_every(std::chrono::microseconds(prop.GetValue(log_config_key::kAsyncFlushEveryMicrosecondInterval,
				log_config_key::default_value::kAsyncFlushEveryMicrosecondIntervalValue)));
		}
		else
		{
			s_logger = std::make_shared<spdlog::logger>(logger_name, std::begin(v_sink), std::end(v_sink));
			
		}

	    int backtrace_size = prop.GetValue(log_config_key::kBacktraceSize, log_config_key::default_value::kBacktraceSizeValue);
        if (backtrace_size == 0)
        {
            s_logger->disable_backtrace();
        }
        else
        {
            s_logger->enable_backtrace(backtrace_size);
            // s_logger->dump_backtrace();
        }

		spdlog::register_logger(s_logger);
		spdlog::set_default_logger(s_logger);

		//spdlog::set_pattern(prop.GetValue(log_config_key::kLoggerPattern, log_config_key::default_value::kLoggerPatternValue));
        s_logger->set_pattern(prop.GetValue(log_config_key::kLoggerPattern, log_config_key::default_value::kLoggerPatternValue));
		std::string log_level = prop.GetValue(log_config_key::kLoggerLevel, log_config_key::default_value::kLoggerLevelValue);
		auto level = spdlog::level::from_str(log_level);
		//spdlog::set_level(level);
        s_logger->set_level(level);

		s_logger_raw = s_logger.get();
		return static_cast<int32_t>(ErrorCode::kSuccess);
	}
}