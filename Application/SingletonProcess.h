#pragma once

#include <stdint.h>
#include <string>
#include <fstream>
#include <memory>

namespace boost
{
	namespace interprocess
	{
		class file_lock;
	}
}

namespace general
{

	class SingletonProcess
	{
	public:
		SingletonProcess(const std::string& file_path);

		~SingletonProcess();

		enum class LockResult :int32_t
		{
			kSuccess,
			kAlreadyLock,
			kCreateFileError,
			kFailure
		};


		int32_t Lock();

	private:
		std::string	file_path_;
		bool	is_locked_;
		std::unique_ptr<boost::interprocess::file_lock> file_lock_;
		std::ofstream	pid_file_stream_;
	};

}