#include "SingletonProcess.h"

#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/filesystem.hpp>

namespace general
{

	SingletonProcess::SingletonProcess(const std::string& file_path) :
		file_path_(file_path + ".pid"),
		is_locked_(false),
		file_lock_(nullptr)
	{

	}

	SingletonProcess::~SingletonProcess()
	{
		if (is_locked_&&file_lock_)
		{
			file_lock_->unlock();
		}

		file_lock_.reset();
	}

	int32_t SingletonProcess::Lock()
	{
		try
		{
			if (is_locked_)
				return  (int32_t)LockResult::kAlreadyLock;

			boost::filesystem::path file_path(file_path_);

			boost::filesystem::path dir_path(file_path.parent_path());

			if (!boost::filesystem::exists(dir_path))
			{
				if (!boost::filesystem::create_directory(dir_path))
				{
					return (int32_t)LockResult::kCreateFileError;
				}
			}


			if (!boost::filesystem::exists(file_path))
			{
				std::ofstream pid_file_stream(file_path_.c_str());
			}


			file_lock_.reset(new boost::interprocess::file_lock(file_path_.c_str()));

			if (!(file_lock_->try_lock()))
			{
				return (int32_t)LockResult::kAlreadyLock;
			}


			pid_file_stream_.open(file_path_.c_str());
			pid_file_stream_ << boost::interprocess::ipcdetail::get_current_process_id();
			pid_file_stream_.flush();

			is_locked_ = true;

			return (int32_t)LockResult::kSuccess;

		}
		catch (...)
		{
			return (int32_t)LockResult::kFailure;
		}
	}
}