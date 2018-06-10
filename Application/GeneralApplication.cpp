#include "GeneralApplication.h"

#include <thread>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include "SingletonProcess.h"

static GeneralApplication* g_app_instance = nullptr;
static volatile bool g_is_signal_to_exit = false;

GeneralApplication::GeneralApplication() :options_desc_("Allowed options", 120)
{
	is_running_ = false;
	singleton_process_ = nullptr;

	g_app_instance = this;
}

GeneralApplication::~GeneralApplication()
{
	singleton_process_.reset();
	g_app_instance = nullptr;
}

int32_t GeneralApplication::OnSingletonLockFilePath(std::string& path)
{
	if (is_singleton_)
	{
		if (path.empty())
		{

			path = exe_file_path_;

		}

		return static_cast<int32_t>(ErrorCode::kSuccess);

	}
	else
	{
		return static_cast<int32_t>(ErrorCode::kPassed);
	}
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
	CallOnExit on_exit([]()
	{
		LOG_INFO("application exit");
	});

	(void)on_exit;
	try
	{
		argc_ = argc;
		argv_ = argv;

		boost::filesystem::path exe_full_path(argv_[0]);
		exe_file_path_ = exe_full_path.parent_path().string();
		exe_file_name_ = exe_full_path.filename().string();

		std::string default_app_name = boost::filesystem::path(exe_file_name_).stem().string();

		options_desc_.add_options()
			("help,h", "show this information")
			("daemon,d", "launch application as a daemon process")
			("singleton,s", "enable singleton file lock")
			("name,n", boost::program_options::value<std::string>(&app_name_)->default_value(default_app_name), "set the application name,default exe name")
			("log-dir", boost::program_options::value<std::string>(&log_info_.log_dir_)->default_value(exe_file_path_), "set the application log directory path,default exe path")
			("log-level,l", boost::program_options::value<std::string>(&log_info_.log_level_)->default_value("Info"), "set the application log level,default Info")
			("log-to-console", "enable log to console")
			;

		SetProgramOption();
		if (!ParseProgramOption())
			return;

		if (OnParseProgramOptionEnd() != static_cast<int32_t>(ErrorCode::kSuccess))
			return;
	

		OnLogInit(log_info_);
		//init log

		std::string lock_dir_path;
		if (OnSingletonLockFilePath(lock_dir_path) == static_cast<int32_t>(ErrorCode::kSuccess))
		{
			std::string file_path = lock_dir_path;

			if (boost::filesystem::is_directory(lock_dir_path))
			{
				file_path += boost::filesystem::path("/").make_preferred().string() + GetAppLicationName();
			}

			singleton_process_.reset(new SingletonProcess(file_path));
			auto ret = singleton_process_->Lock();
			if ( ret!= static_cast<int32_t>(SingletonProcess::LockResult::kSuccess))
			{
				LOG_ERROR("singleton process lock failed [{0}],application {1} may be already running",ret, app_name_);
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
		}

		if (OnDaemonizeEnd() != static_cast<int32_t>(ErrorCode::kSuccess))
		{
			LOG_ERROR("OnDaemonizeEnd not return success");
			return;
		}

		if (is_daemonize_)
		{
			RedirectInputOutput();
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

		int32_t ret = 0;
		is_running_ = true;
		while (is_running_)
		{
			try
			{
				ret = OnRun();
			}
			catch (...)
			{
				LOG_ERROR("application throw exception in <OnRun> :{0}",boost::current_exception_diagnostic_information());
				Stop();
			}

			if (ret == static_cast<int32_t>(ErrorCode::kSuccess)
				|| ret == static_cast<int32_t>(ErrorCode::kPassed))
			{
				OnIdle();
				continue;
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
		//
		//boost::current_exception_diagnostic_information();
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

#if defined(C_SYSTEM_LINUX)
void GenericApplication::SignalHandler(int sig_num, siginfo_t* sig_info, void* ptr)
{
	if (g_app_instance == nullptr)
		return;
	if (sig_num == SIGUSR1)
	{
		if (sig_info != nullptr)
		{
			switch (sig_info->si_int)
			{
			case 0:
				g_is_signal_to_exit = true;

			default:
				break;
			}
		}
	}

	g_app_instance->OnSignal(sig_num, sig_info->sig_int);
}

bool GenericApplication::InstalSignalHandler()
{
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		LOG_ERROR("ignore signal SIGPIPE failed");
		return false;
	}
	if (signal(SIGHUB, SIG_IGM) == SIG_ERR)
	{
		LOG_ERROR("ignore signal SIGHUB failed");
		return false;
	}
	struct  sigaction sig_action;
	sig_action.sa_handler = nullptr;
	sig_action.sa_sigaction = &GenericApplication::SignalHandler;
	sig_action.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_restorer = nullptr;

	int ret = sigaction(SIGUSR1, &sig_action, nullptr);
	if (ret != 0)
	{
		LOG_ERROR("sigaction on SIGUSR1  failed,error:{0},error string:{1}",error,strerror(error));
		return false;
	}

	int ret = sigaction(SIGUSR2, &sig_action, nullptr);
	if (ret != 0)
	{
		LOG_ERROR("sigaction on SIGUSR2  failed,error:{0},error string:{1}", error, strerror(error));
		return false;
	}

	return true;

#elif defined(C_SYSTEM_WINDOWS)
#endif

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
		std::cout << "usage:" << std::endl;
		std::cout << options_desc_ << std::endl;
		return false;
	}

	if (options_vm_.count("log-to-console"))
	{
		log_info_.is_console_log_ = true;
	}

	if (options_vm_.count("daemon"))
	{
		is_daemonize_ = true;
	}

	if (options_vm_.count("singleton"))
	{
		is_singleton_ = true;
	}





	for (auto& item : option_callback_map_)
	{
		if (options_vm_.count(item.first))
		{
			auto ec = item.second();
			if (ec != static_cast<int32_t>(ErrorCode::kSuccess))
			{
				return false;
			}
		}
	}

	for (auto& option_name : option_name_set_)
	{
		if (options_vm_.count(option_name))
		{
			OnProgramOption(option_name);
		}
	}

	return true;
}

void GeneralApplication::RedirectInputOutput()
{
#if defined(C_SYSTEM_LINUX)
	close(STDIN_FILENO);
	close(STDIN_FILENO);
	close(STDIN_FILENO);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);
#elif defined(C_SYSTEM_WINDOWS)
#endif
}

