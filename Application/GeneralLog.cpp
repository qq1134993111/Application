#include "GeneralLog.h"
#include <boost/filesystem.hpp>
namespace general
{
	std::shared_ptr<spdlog::logger> GeneralLog::s_logger = nullptr;
	std::shared_ptr<spdlog::logger> GeneralLog::s_console = nullptr;


	int32_t GeneralLog::Init(const Property& prop)
	{
		if (s_logger == nullptr)
		{

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


			std::unordered_map<uint8_t,
				std::function<std::shared_ptr<spdlog::logger>(const std::string& logger_name, const spdlog::filename_t& filename, bool truncate)>
			> basic_logger_map
			{
				{ static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerSt),spdlog::basic_logger_st },
				{ static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerMt),spdlog::basic_logger_mt }

			};

			std::unordered_map<uint8_t,
				std::function<std::shared_ptr<spdlog::logger>(const std::string& logger_name, const spdlog::filename_t& filename, size_t max_file_size, size_t max_files)>
			> rotating_logger_map
			{
				{ static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerSt),spdlog::rotating_logger_st },
				{ static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerMt),spdlog::rotating_logger_mt }

			};

			std::unordered_map<uint8_t,
				std::function<std::shared_ptr<spdlog::logger>(const std::string& logger_name, const spdlog::filename_t& filename, int hour, int minute)>
			> daily_logger_map
			{
				{ static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerSt),spdlog::daily_logger_st },
				{ static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerMt),spdlog::daily_logger_mt }

			};

			auto logger_thread_mode = prop.GetValue(log_config_key::kLoggerThreadMode, log_config_key::default_value::kLoggerThreadModeValue);
			auto logger_type = prop.GetValue(log_config_key::kLoggerType, log_config_key::default_value::kLoggerTypeValue);
			switch (logger_type)
			{
			case static_cast<uint8_t>(log_config_key::LoggerType::kLoggerTypeBasic) :
				s_logger = basic_logger_map[logger_thread_mode](logger_name, logger_file_name, prop.GetValue(log_config_key::kBasicTruncate, log_config_key::default_value::kBasicTruncateValue));
				break;
			case static_cast<uint8_t>(log_config_key::LoggerType::kLoggerTypeRotating) :
				s_logger = rotating_logger_map[logger_thread_mode](logger_name, logger_file_name,
					prop.GetValue(log_config_key::kRotatingMaxFileSize, log_config_key::default_value::kRotatingMaxFileSizeValue),
					prop.GetValue(log_config_key::kRotatingMaxFiles, log_config_key::default_value::kRotatingMaxFilesValue)
					);
				break;
			case static_cast<uint8_t>(log_config_key::LoggerType::kLoggerTypeDaily) :
				s_logger = daily_logger_map[logger_thread_mode](logger_name, logger_file_name,
					prop.GetValue(log_config_key::kDailyHour, log_config_key::default_value::kDailyHourValue),
					prop.GetValue(log_config_key::kDailyMinute, log_config_key::default_value::kDailyMinuteValue)
					);
				break;
			default:
				break;
			}

			if (prop.GetValue(log_config_key::kUseConsoleLogger, log_config_key::default_value::kUseConsoleLoggerValue))
			{
				std::unordered_map<uint8_t,
					std::function<std::shared_ptr<spdlog::logger>(const std::string& logger_name)>
				> stdout_logger_map
				{
					{ static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerSt),spdlog::stdout_color_st },
					{ static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerMt),spdlog::stdout_color_mt }
				};

				s_console = stdout_logger_map[logger_thread_mode]("console");
			}

			spdlog::set_pattern(prop.GetValue(log_config_key::kLoggerPattern, log_config_key::default_value::kLoggerPatternValue));

			if (prop.GetValue(log_config_key::kAsyncMode, log_config_key::default_value::kAsyncModeValue))
			{
				spdlog::set_async_mode(prop.GetValue(log_config_key::kAsyncQueueSize, log_config_key::default_value::kAsyncQueueSizeValue));
			}
			else
			{
				spdlog::set_sync_mode();
			}

			std::string log_level = prop.GetValue(log_config_key::kLoggerLevel, log_config_key::default_value::kLoggerLevelValue);
			auto level = spdlog::level::from_str(log_level);
			spdlog::set_level(level);
			

			return static_cast<int32_t>(ErrorCode::kSuccess);
		}

		return  static_cast<int32_t>(ErrorCode::kAlreadyInit);
	}


}