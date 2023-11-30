#pragma once
#include <stdint.h>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <memory>
#include <atomic>

#include "CPreDefined.h"

#if defined(C_SYSTEM_GNU_LINUX)
#include <errno.h>
#include <signal.h>
#elif defined(C_SYSTEM_WINDOWS) 
//#include <signal.h>
#endif 

#include <boost/program_options.hpp>
#include <boost/optional.hpp>
#include <boost/exception/diagnostic_information.hpp>



#include "GeneralLog.h"

namespace general
{
	class SingletonProcess;

	class GeneralApplication
	{
	public:
		enum class ErrorCode :int32_t
		{
			kSuccess,
			kPassed,
			kInvalidOptionName,
			kDuplicatedOptionName,
			kFailure
		};

		using OptionCallbackNoArg = std::function<int32_t(const std::string& option_name)>;

		template<typename ArgumentType>
		using OptionCallback = std::function<int32_t(const std::string& option_name, const ArgumentType& value)>;

	public:
		GeneralApplication();
		virtual ~GeneralApplication();


		virtual void SetProgramOption() {}
		virtual void OnProgramOption(const std::string& option_name) {}
		virtual int32_t OnParseProgramOptionEnd() { return static_cast<int32_t>(ErrorCode::kSuccess); }

		virtual int32_t OnSingletonLockFilePath(std::string& path);
		virtual int32_t OnDaemonizeEnd() { return static_cast<int32_t>(ErrorCode::kSuccess); }


		virtual int32_t OnLogInit(Property& log_prop) { return static_cast<int32_t>(ErrorCode::kPassed); }
		virtual int32_t OnInit() { return static_cast<int32_t>(ErrorCode::kSuccess); }
        virtual int32_t OnStart(){return static_cast<int32_t>(ErrorCode::kSuccess); }
		virtual int32_t OnRun() { return static_cast<int32_t>(ErrorCode::kPassed); }
		virtual void OnIdle();
		virtual void OnSignal(int sig_num, int value) {}
		virtual void OnExit() {}

		void Run(int argc, char* argv[]);

		bool IsRunning()
		{
			return is_running_;
		}

		void Stop()
		{
			is_running_ = false;
		}

		const std::string& GetExeFilePath()
		{
			return exe_file_path_;
		}

		const std::string& GetAppLicationName()
		{
			return app_name_;
		}
	protected:
		int32_t AddOption(const char* option_name, const char* option_desc)
		{
			int32_t ec;
			if ((ec = OptionCheck(option_name)) != static_cast<int32_t>(ErrorCode::kSuccess))
			{
				return ec;
			}

			options_desc_.add_options()(option_name, option_desc);

			return static_cast<int32_t>(ErrorCode::kSuccess);
		}

		template<typename ArgumentType>
		int32_t AddOptionWithArgument(const char* option_name, const char* option_desc, ArgumentType* p_store_value = nullptr)
		{
			int32_t ec;
			if ((ec = OptionCheck(option_name)) != static_cast<int32_t>(ErrorCode::kSuccess))
			{
				return ec;
			}

			options_desc_.add_options()
				(option_name, boost::program_options::value<ArgumentType>(p_store_value), option_desc);

			return static_cast<int32_t>(ErrorCode::kSuccess);
		}

		template<typename ArgumentType>
		int32_t AddOptionWithArgument(const char* option_name, const char* option_desc, const ArgumentType& default_value, ArgumentType* p_store_value = nullptr)
		{
			int32_t ec;
			if ((ec = OptionCheck(option_name)) != static_cast<int32_t>(ErrorCode::kSuccess))
			{
				return ec;
			}

			options_desc_.add_options()
				(option_name, boost::program_options::value<ArgumentType>(p_store_value)->default_value(default_value), option_desc);

			return static_cast<int32_t>(ErrorCode::kSuccess);
		}


		int32_t AddOptionWithCallback(const char* option_name, const char* option_desc, const OptionCallbackNoArg& callback)
		{
			int32_t ec;
			if ((ec = OptionCheck(option_name)) != static_cast<int32_t>(ErrorCode::kSuccess))
			{
				return ec;
			}

			std::string long_option_name = GetLongOptionName(option_name);
			option_callback_map_.emplace(long_option_name, 
				[=]()->int32_t 
				{
				   return  callback(long_option_name);
				});

			options_desc_.add_options()(option_name, option_desc);

			return static_cast<int32_t>(ErrorCode::kSuccess);
		}

		template<typename ArgumentType>
		int32_t AddOptionWithCallback(const char* option_name, const char* option_desc, const OptionCallback<ArgumentType>& callback)
		{
			int32_t ec;
			if ((ec = OptionCheck(option_name)) != static_cast<int32_t>(ErrorCode::kSuccess))
			{
				return ec;
			}

			std::string long_option_name = GetLongOptionName(option_name);
			option_callback_map_.emplace(long_option_name, 
				[=]()->int32_t 
				{
				   return callback(long_option_name, GetOptionArgument<ArgumentType>(long_option_name));
				});

			options_desc_.add_options()(option_name, boost::program_options::value<ArgumentType>(), option_desc);

			return static_cast<int32_t>(ErrorCode::kSuccess);
		}

		template<typename ArgumentType>
		int32_t AddOptionWithCallback(const char* option_name, const char* option_desc, const ArgumentType& default_value, const OptionCallback<ArgumentType>& callback)
		{
			int32_t ec;
			if ((ec = OptionCheck(option_name)) != static_cast<int32_t>(ErrorCode::kSuccess))
			{
				return ec;
			}

			std::string long_option_name = GetLongOptionName(option_name);
			option_callback_map_.emplace(long_option_name, 
				[=]()->int32_t
				{
				   return callback(long_option_name, GetOptionArgument<ArgumentType>(long_option_name));
				});

			options_desc_.add_options()(option_name, boost::program_options::value<ArgumentType>()->default_value(default_value), option_desc);

			return static_cast<int32_t>(ErrorCode::kSuccess);
		}

		template<typename ArgumentType>
		ArgumentType GetOptionArgument(const std::string& option_name)
		{
			try
			{
				return options_vm_[option_name].as<ArgumentType>();
			}
			catch (...)
			{
				throw std::runtime_error(std::string("GetOptionArgument failed <" + option_name) + "> ," + boost::current_exception_diagnostic_information());
			}
		}

		template<typename ArgumentType>
		boost::optional<ArgumentType> GetOptionArgumentOptional(const std::string& option_name)
		{
			try
			{
				return options_vm_[option_name].as<ArgumentType>();
			}
			catch (...)
			{
				return {};
			}
		}

	private:
#if defined(C_SYSTEM_GNU_LINUX) 
		static void SignalHandler(int sig_num, siginfo_t* sig_info, void* ptr);
		static bool InstalSignalHandler();
#elif defined(C_SYSTEM_WINDOWS)
#endif
		std::string GetLongOptionName(const std::string& option_name);
		int32_t OptionCheck(const char* option_name);
		bool ParseProgramOption();
		void RedirectInputOutput();
	private:

		int argc_;
		char** argv_;

		std::string exe_file_name_;
		std::string exe_file_path_;
		std::string app_name_;
		
		std::atomic<bool> is_running_{ false };

		bool is_daemonize_{ false };
		bool is_singleton_{ false };

		Property log_prop_;
	
		std::set<std::string> option_name_set_;
		std::map<std::string, std::function<int32_t()>> option_callback_map_;
		boost::program_options::options_description options_desc_;
		boost::program_options::variables_map options_vm_;

		std::unique_ptr<SingletonProcess> singleton_process_;

	};

}