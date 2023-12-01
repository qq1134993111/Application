#include "GeneralApplication.h"

#include <thread>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include "SingletonProcess.h"
namespace general
{
	//通用应用程序基类
	static GeneralApplication* g_app_instance = nullptr;
	static volatile bool g_is_signal_to_exit = false;

#if defined(C_SYSTEM_GNU_LINUX)
	void GeneralApplication::SignalHandler(int sig_num, siginfo_t* sig_info, void* ptr)
	{
		if (g_app_instance == nullptr)
			return;

		if (sig_num == SIGUSR1)
		{
			if (sig_info != nullptr)
			{
				switch (sig_info->si_int)
				{
				case -1:
				{
					g_is_signal_to_exit = true;
				}
				break;
				case spdlog::level::trace:
				case spdlog::level::debug:
				case spdlog::level::info:
				case spdlog::level::warn:
				case spdlog::level::err:
				case spdlog::level::critical:
				{
					//1-6
					LOG_SET_LEVEL(static_cast<spdlog::level::level_enum>(sig_info->si_int));
				}
				break;

				default:
					break;
				}
			}
		}

		g_app_instance->OnSignal(sig_num, sig_info->si_int);
	}

	bool GeneralApplication::InstalSignalHandler()
	{
		if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		{
			LOG_ERROR("ignore signal SIGPIPE failed");
			return false;
		}
		if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
		{
			LOG_ERROR("ignore signal SIGHUB failed");
			return false;
		}
		struct  sigaction sig_action;
		sig_action.sa_handler = nullptr;
		sig_action.sa_sigaction = &GeneralApplication::SignalHandler;
		sig_action.sa_flags = SA_RESTART | SA_SIGINFO;
		sigemptyset(&sig_action.sa_mask);
		sig_action.sa_restorer = nullptr;

		int ret = sigaction(SIGUSR1, &sig_action, nullptr);
		if (ret != 0)
		{
			LOG_ERROR("sigaction on SIGUSR1  failed,error:{0},error string:{1}", errno, strerror(errno));
			return false;
		}

		ret = sigaction(SIGUSR2, &sig_action, nullptr);
		if (ret != 0)
		{
			LOG_ERROR("sigaction on SIGUSR2  failed,error:{0},error string:{1}", errno, strerror(errno));
			return false;
		}

		return true;
	}

#elif defined(C_SYSTEM_WINDOWS)
#endif
	GeneralApplication::GeneralApplication()
	{
		is_running_ = false;
		singleton_process_ = nullptr;

		g_app_instance = this;
	}

	GeneralApplication::~GeneralApplication()
	{
		singleton_process_.reset();
		g_app_instance = nullptr;
        LOG_SHUTDOWN();
	}

	int32_t GeneralApplication::OnSingletonLockFilePath(std::string& path)
	{
		return static_cast<int32_t>(ErrorCode::kPassed);
	}

	class CallOnExit
	{
	public:
		CallOnExit(const std::function<void()>& on_exit) :call_on_exit_(on_exit) {}
		~CallOnExit() { call_on_exit_(); }

	private:
		std::function<void()> call_on_exit_;
	};


	void GeneralApplication::OnIdle()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	void GeneralApplication::Run(int argc, char* argv[])
	{
		CallOnExit on_exit(
			[]()
			{
				std::cout << "application exit" << std::endl;
				LOG_INFO("application exit");
				LOG_FLUSH_ON_ALL();
			});

		(void)on_exit;
		try
		{
			argc_ = argc;
			argv_ = argv;

			boost::filesystem::path exe_full_path(argv_[0]);
            exe_full_path = boost::filesystem::weakly_canonical(exe_full_path);
			
			exe_file_path_ = boost::filesystem::absolute(exe_full_path.parent_path()).string();
			exe_file_name_ = exe_full_path.filename().string();

			std::string default_app_name = boost::filesystem::path(exe_file_name_).stem().string();

			std::string default_log_dir = exe_file_path_ + boost::filesystem::path("/").make_preferred().string() + "log";
            //boost::filesystem::path path1(exe_file_path_);
            //auto path2 = path1 / "log";
            //path1 /= "log";
            //assert(path1 == path2);
            //default_log_dir = path1.string();

			decltype(options_desc_) desc(default_app_name + " options", 210);
			options_desc_.add(desc);
			options_desc_.add_options()
				("help,h", "show this information")
				("daemon,d", "launch application as a daemon process")
				("singleton,s", "enable singleton file lock")
				("name,n", boost::program_options::value<std::string>(&app_name_)->default_value(default_app_name),
                fmt::format("set the application name, default {}", default_app_name).c_str())
				("log-dir", boost::program_options::value<std::string>()->default_value(default_log_dir),
                fmt::format("set the application log directory path,default {}", default_log_dir).c_str())
				("log-level", boost::program_options::value<std::string>()->default_value("info"), "set the application log level,[trace,debug, info, warning, error,critical,off],default info")
				("log-to-console", "enable log to console")
				;

			AddOptionWithCallback<std::string>("log-thread-mode", "set log thread mode,single thread or multi thread [st,mt],default mt",std::string("mt"),
				[this](const std::string& option_name,const std::string& thread_mode)->int32_t
				{
                    std::cout << log_config_key::kLoggerThreadMode << ":" << thread_mode << "\n";
                    if (thread_mode == "st")
                    {
                        log_prop_.SetValue(log_config_key::kLoggerThreadMode,
                                           static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerSt));
                    }
                    else if (thread_mode == "mt")
                    {
                        log_prop_.SetValue(log_config_key::kLoggerThreadMode,
                                           static_cast<uint8_t>(log_config_key::LoggerThreadMode::kLoggerMt));
                    }
                    else
                    {
                        return static_cast<int32_t>(ErrorCode::kFailure);
                    }
                 
					return static_cast<int32_t>(ErrorCode::kSuccess);
				});

			AddOptionWithCallback("log-async", "enable async log",
				[this](const std::string& option_name)->int32_t
				{
					log_prop_.SetValue(log_config_key::kAsyncMode, true);
					return static_cast<int32_t>(ErrorCode::kSuccess);
				});

			AddOptionWithCallback<uint32_t>("log-async-queue-size",
                fmt::format("async log queue size,default {}", log_config_key::default_value::kAsyncQueueSizeValue).c_str(),
				[this](const std::string& option_name, uint32_t size)->int32_t
				{
                    std::cout << log_config_key::kAsyncQueueSize << ":" << size << "\n";
					log_prop_(log_config_key::kAsyncQueueSize, size);
					return static_cast<int32_t>(ErrorCode::kSuccess);
				});

            AddOptionWithCallback<uint32_t>("log-async-thread-count",
                                            fmt::format("async log thread count,default {}", log_config_key::default_value::kAsyncThreadCountValue).c_str(),
                                            [this](const std::string &option_name, uint32_t count) -> int32_t {

                                                std::cout << log_config_key::kAsyncThreadCount << ":" << count << "\n";
                                                log_prop_(log_config_key::kAsyncThreadCount, count);
                                                return static_cast<int32_t>(ErrorCode::kSuccess);
                                            });

            AddOptionWithCallback<uint64_t>(
                "log-async-flush-every-microsecond-interval",
                fmt::format("async log flush every microsecond interval,default {} microsecond", log_config_key::default_value::kAsyncFlushEveryMicrosecondIntervalValue).c_str(),
                [this](const std::string &option_name,uint64_t microsecond_interval) -> int32_t {
                    std::cout << log_config_key::kAsyncFlushEveryMicrosecondInterval << ":" << microsecond_interval << "\n";
                    log_prop_(log_config_key::kAsyncFlushEveryMicrosecondInterval, microsecond_interval);
                    return static_cast<int32_t>(ErrorCode::kSuccess);
                });

			AddOptionWithCallback<std::string>("log-type", "log type,[basic,rotating,daily],default daily", "daily",
				[this](const std::string& option_name, const std::string& value)->int32_t
				{
                    std::cout << log_config_key::kLoggerType << ":" << value << "\n";

					std::unordered_map<std::string, uint8_t> umap
					{
						{"basic",static_cast<uint8_t>(log_config_key::LoggerType::kLoggerTypeBasic)},
						{ "rotating",static_cast<uint8_t>(log_config_key::LoggerType::kLoggerTypeRotating) },
						{ "daily",static_cast<uint8_t>(log_config_key::LoggerType::kLoggerTypeDaily) }
					};

					auto level = boost::algorithm::to_lower_copy(value);

					auto it = umap.find(level);
					if (it == umap.end())
					{
                        std::cout << fmt::format("log-type {} not found", value);
						return  static_cast<int32_t>(ErrorCode::kFailure);
					}

					log_prop_.SetValue(log_config_key::kLoggerType, it->second);

					return static_cast<int32_t>(ErrorCode::kSuccess);
				});

			AddOptionWithCallback<bool>("basic-truncate",
                                        fmt::format("basic log whether to truncate the original file. default {}",log_config_key::default_value::kBasicTruncateValue).c_str(),
                                        [this](const std::string &option_name, const bool &value) {
                                            std::cout << log_config_key::kBasicTruncate << ":" << value << "\n";
                                            log_prop_(log_config_key::kBasicTruncate, value);
                                            return static_cast<int32_t>(ErrorCode::kSuccess);
                                        });

			auto fun_rotate_set = [this](const std::string& option_name, const uint64_t& value)
			{
				if (option_name == "rotating-max-file-size")
				{
                    std::cout << log_config_key::kRotatingMaxFileSize << ":" << value << "\n";
					log_prop_(log_config_key::kRotatingMaxFileSize, value);
				}
				else if (option_name == "rotating-max-files")
				{
                    std::cout << log_config_key::kRotatingMaxFiles << ":" << value << "\n";
					log_prop_(log_config_key::kRotatingMaxFiles, value);
				}
				return static_cast<int32_t>(ErrorCode::kSuccess);
			};

            AddOptionWithCallback<uint64_t>("rotating-max-file-size",fmt::format("rotating log max file size,default {} bytes", log_config_key::default_value::kRotatingMaxFileSizeValue).c_str(),fun_rotate_set);
            AddOptionWithCallback<uint64_t>("rotating-max-files", fmt::format("rotating log max files,default {} files",log_config_key::default_value::kRotatingMaxFilesValue).c_str(), fun_rotate_set);

			auto fun_daily_set = [this](const std::string& option_name, const std::string& value)
			{
                std::cout << option_name << ":" << value << "\n";
				std::vector<std::string> splits;
				boost::algorithm::split(splits, value, boost::algorithm::is_any_of(":"), boost::algorithm::token_compress_on);
				if (splits.size() != 2)
				{
					return static_cast<int32_t>(ErrorCode::kFailure);
				}

				log_prop_(log_config_key::kDailyHour, boost::lexical_cast<int32_t>(splits[0]));
				log_prop_(log_config_key::kDailyMinute, boost::lexical_cast<int32_t>(splits[1]));

				return static_cast<int32_t>(ErrorCode::kSuccess);
			};
			AddOptionWithCallback<std::string>("daily_time", "daily log will create a new log file each day on HH:mm,default 00:00", "00:00", fun_daily_set);


			AddOptionWithCallback<std::string>(
                "log-pattern",
                fmt::format("log pattern,default {}", general::log_config_key::default_value::kLoggerPatternValue).c_str(), 
				[this](const std::string& option_name, const std::string& value)->int32_t
				{
                    std::cout << log_config_key::kLoggerPattern << ":" << value << "\n";
					log_prop_(log_config_key::kLoggerPattern, value);
					return static_cast<int32_t>(ErrorCode::kSuccess);
				});

			AddOptionWithCallback<uint16_t>(
                "log-backtrace-size",
                fmt::format("enable log backtrace and set size,0 is disable . default {}",log_config_key::default_value::kBacktraceSizeValue).c_str(),
                [this](const std::string &option_name, uint16_t n_message) -> int32_t 
				{
                    std::cout << log_config_key::kBacktraceSize << ":" << n_message << "\n";
                    log_prop_(log_config_key::kBacktraceSize, n_message);
                    return static_cast<int32_t>(ErrorCode::kSuccess);
                });

			//AddOptionWithArgument<std::string>("name,n", "set the application name,default exe name", default_app_name, &app_name_);

			SetProgramOption();
			if (!ParseProgramOption())
				return;

			if (OnParseProgramOptionEnd() != static_cast<int32_t>(ErrorCode::kSuccess))
			{
				std::cout << "OnParseProgramOptionEnd not return kSuccess" << std::endl;
				return;
			}


			OnLogInit(log_prop_);

			if (LOG_INIT(log_prop_) != 0)
			{
				std::cout << "log init failed" << std::endl;
				return;
			}

			std::string cmdline;
			for (int i = 0; i < argc_; i++)
			{
				cmdline += argv_[i] + std::string(" ");
			}
			LOG_INFO("Command line parameter:{}", cmdline);
			LOG_DEBUG("exe path:{0},exe name:{1},app name:{2}", exe_file_path_,exe_file_name_,app_name_);
			std::string lock_path;
			if (OnSingletonLockFilePath(lock_path) == static_cast<int32_t>(ErrorCode::kSuccess) || is_singleton_)
			{
				if (lock_path.empty())
					lock_path = exe_file_path_;

				if (boost::filesystem::is_directory(lock_path))
				{
					lock_path += boost::filesystem::path("/lock/").make_preferred().string() + GetAppLicationName();
				}

				std::string file_path = lock_path;
				LOG_DEBUG("lock file path:{}", file_path);
				singleton_process_.reset(new SingletonProcess(file_path));
				auto ret = singleton_process_->Lock();
				if (ret != static_cast<int32_t>(SingletonProcess::LockResult::kSuccess))
				{
					LOG_ERROR("singleton process lock failed [{0}],application {1} may be already running", ret, app_name_);
					return;
				}
			}

			if (is_daemonize_)
			{
#if defined(C_SYSTEM_LINUX)
				if (daemon(1, 1) != 0)
				{
					LOG_ERROR("daemon failed,{0}:{1}", errno, strerror(errno));
					return;
				}
#elif defined(C_SYSTEM_WINDOWS)
#endif

				RedirectInputOutput();
			}

			if (OnDaemonizeEnd() != static_cast<int32_t>(ErrorCode::kSuccess))
			{
				LOG_ERROR("OnDaemonizeEnd not return success");
				return;
			}

#if defined(C_SYSTEM_LINUX)
			if (!InstalSignalHandler())
			{
				LOG_ERROR("install signal handler failed");
				return;
			}
#elif defined(C_SYSTEM_WINDOWS)
#endif

	
            if (OnInit() != static_cast<int32_t>(ErrorCode::kSuccess))
            {
                LOG_ERROR("application init failed");
                return;
            }
            else
            {
                LOG_INFO("application init successfully");
            }

            is_running_ = true;

            if (OnStart() != static_cast<int32_t>(ErrorCode::kSuccess))
            {
                LOG_ERROR("application start failed");
                return;
            }
            else
            {
                LOG_INFO("application start successfully");
            }


			LOG_FLUSH_ON_ALL();

			int32_t ret = 0;
            while (is_running_)
            {
                try
                {
                    ret = OnRun();
                }
                catch (...)
                {
                    LOG_ERROR("application throw exception in <OnRun> :{0} ,will stop",boost::current_exception_diagnostic_information());
                    Stop();
                }

                if (ret == static_cast<int32_t>(ErrorCode::kSuccess)/*|| ret == static_cast<int32_t>(ErrorCode::kPassed)*/)
                {

                    OnIdle();
                }
                else
                {
                    Stop();
                }
            }


			if (g_is_signal_to_exit)
			{
				LOG_INFO("application is exit by signal");
			}

			try
			{
				OnExit();
			}
			catch (...)
			{
				LOG_ERROR("application throw exception in <OnExit> :{0}", boost::current_exception_diagnostic_information());
			}

		}
		catch (...)
		{
			//std::cout << "application throw exception in <Run> :" << boost::current_exception_diagnostic_information() << std::endl;

			LOG_ERROR("application throw exception in <Run> :{0}", boost::current_exception_diagnostic_information());

			try
			{
				OnExit();
			}
			catch (...)
			{
				LOG_ERROR("application throw exception in <OnExit> :{0}", boost::current_exception_diagnostic_information());
			}
		}
	}

	std::string GeneralApplication::GetLongOptionName(const std::string& option_name)
	{
		std::vector<std::string> splits;
		boost::algorithm::split(splits, option_name, boost::algorithm::is_any_of(","), boost::algorithm::token_compress_on);
		return std::move(splits[0]);
	}

	int32_t GeneralApplication::OptionCheck(const char* option_name)
	{
		std::string long_option_name = GetLongOptionName(option_name);
		if (long_option_name.empty())
		{
			return static_cast<int32_t>(ErrorCode::kInvalidOptionName);
		}

		if (option_name_set_.find(long_option_name) != option_name_set_.end())
		{
			return  static_cast<int32_t>(ErrorCode::kDuplicatedOptionName);
		}

		option_name_set_.insert(long_option_name);

		return static_cast<int32_t>(ErrorCode::kSuccess);
	}

	bool GeneralApplication::ParseProgramOption()
	{
		boost::program_options::store(boost::program_options::parse_command_line(argc_, argv_, options_desc_), options_vm_);
		boost::program_options::notify(options_vm_);
		if (options_vm_.count("help"))
		{
			//std::cout << "----------------------------------------------------------\n";
			std::cout << "usage:" << std::endl;
			std::cout << options_desc_ << std::endl;
			//std::cout << "----------------------------------------------------------\n";
			return false;
		}

		if (options_vm_.count("log-to-console"))
		{
            std::cout << "log-to-console\n";
			log_prop_.SetValue(log_config_key::kUseConsoleLogger, true);
		}

		auto log_dir = GetOptionArgumentOptional<std::string>("log-dir");
		if (log_dir)
		{
            boost::filesystem::path dir = *log_dir + boost::filesystem::path("/").string() + GetAppLicationName() + ".log";
            dir = boost::filesystem::weakly_canonical(dir);
            std::cout << "log-dir:" << dir.string() << "\n";

			log_prop_.SetValue(log_config_key::kLoggerFilename,dir.string());
		}

		auto log_level = GetOptionArgumentOptional<std::string>("log-level");
		if (log_level)
		{
            std::cout << "log-level:" << *log_level << "\n";
			log_prop_(log_config_key::kLoggerLevel, *log_level);
		}


		if (options_vm_.count("daemon"))
		{
            std::cout << "daemon\n";
			is_daemonize_ = true;
		}

		if (options_vm_.count("singleton"))
		{
            std::cout << "singleton\n";
			is_singleton_ = true;
		}


		for (auto& item : option_callback_map_)
		{
			if (options_vm_.count(item.first))
			{
				auto ec = item.second();
				if (ec != static_cast<int32_t>(ErrorCode::kSuccess))
				{
					std::cout << "ParseProgramOption error,option_callback_map_ key [" << item.first << "] call failed\n";
                    std::cout << "usage:" << std::endl;
                    std::cout << options_desc_ << std::endl;
					return false;
				}
			}
		}

		for (auto& option_name : option_name_set_)
		{
			if (options_vm_.count(option_name))
			{
                //std::cout << option_name << "\n";
				OnProgramOption(option_name);
			}
		}

		return true;
	}

	void GeneralApplication::RedirectInputOutput()
	{
#if defined(C_SYSTEM_GNU_LINUX)
		//close(STDIN_FILENO);
		//close(STDIN_FILENO);
		//close(STDIN_FILENO);
		//open("/dev/null", O_RDWR);
		//open("/dev/null", O_RDWR);
		//open("/dev/null", O_RDWR);
#elif defined(C_SYSTEM_WINDOWS)
#endif
}

}