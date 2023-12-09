#include "DumpHelper.h"

#if defined(C_SYSTEM_WINDOWS)

#include <boost/filesystem.hpp>
#include <functional>
#include <memory>

#include <windows.h>
#include <DbgHelp.h>
// #pragma comment(lib,"Dbghelp.lib")

LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo);

static std::string s_dump_directory_;
static std::string s_dump_file_name_;

bool DumpHelper::EnableDump(std::string directory, std::string file_name)
{
    boost::system::error_code ec;
    boost::filesystem::path path;
    if (directory.empty() || file_name.empty())
    {
        wchar_t szMbsFile[MAX_PATH] = {0};
        ::GetModuleFileNameW(NULL, szMbsFile, MAX_PATH);
        path = szMbsFile;

        if (directory.empty())
        {
            directory = path.parent_path().string();
        }

        if (file_name.empty())
        {
            file_name = path.stem().string();
        }
    }

    path = directory;
    if (path.is_relative())
    {
        path = boost::filesystem::absolute(path, ec);
    }

    path = boost::filesystem::weakly_canonical(path, ec);
    if (boost::filesystem::exists(path,ec))
    {
        if (!boost::filesystem::is_directory(path, ec))
        {
            printf("directory path %s is not a directory\n", path.string().c_str());
            return false;
        }
    }

    s_dump_directory_ = path.string();
    s_dump_file_name_ = file_name;

    printf("s_dump_directory_:%s\n", s_dump_directory_.c_str());
    printf("s_dump_file_name_:%s\n", s_dump_file_name_.c_str());

    SetUnhandledExceptionFilter(ExceptionFilter);
    return true;
}

LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
    // 这里做一些异常的过滤或提示
    if (IsDebuggerPresent())
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    timespec ts;
    timespec_get(&ts, TIME_UTC);
    struct tm tm1 = {0};
    localtime_s(&tm1, &ts.tv_sec);
    char sz_time[64] = {0};
    strftime(sz_time, sizeof(sz_time), "%Y%m%d-%H%M%S", &tm1);
    char sz_file_name[256] = {0};
    snprintf(sz_file_name, sizeof(sz_file_name), "%s_%s-%ld_PID-%d_TID-%d.dmp", s_dump_file_name_.c_str(), sz_time,
             ts.tv_nsec, GetCurrentProcessId(), GetCurrentThreadId());

    boost::system::error_code ec;
    boost::filesystem::path write_path(s_dump_directory_);
    if (!boost::filesystem::exists(write_path, ec))
    {
        if (!boost::filesystem::create_directories(write_path, ec))
        {
            printf("create s_dump_directory_ %s failed,%d:%s\n", write_path.string().c_str(), errno, strerror(errno));
            return EXCEPTION_CONTINUE_SEARCH;
        }
    }

    write_path /= sz_file_name;

    HANDLE hDumpFile = CreateFileW(write_path.wstring().c_str(), GENERIC_READ | GENERIC_WRITE,
                                   FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

    if (INVALID_HANDLE_VALUE == hDumpFile)
    {
        int error = GetLastError();
        std::string error_message = std::system_category().message(error);
        printf("CreateFileW %s failed,%d:%s\n", write_path.string().c_str(), error, error_message.c_str());

        return EXCEPTION_CONTINUE_EXECUTION;
    }
    std::unique_ptr<HANDLE, void (*)(HANDLE *)> ptr_file_handle(&hDumpFile, [](HANDLE *p_h) {
        if (*p_h != INVALID_HANDLE_VALUE)
        {
            CloseHandle(*p_h);
        }
    });

    // 定义函数指针
    typedef BOOL(WINAPI * MiniDumpWriteDumpT)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION,
                                              PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);

    // 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数
    MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
    HMODULE hDbgHelp = LoadLibraryW(L"DbgHelp.dll");
    if (NULL == hDbgHelp)
    {
        int error = GetLastError();
        std::string error_message = std::system_category().message(error);
        printf("LoadLibraryW DbgHelp.dll failed,%d:%s\n", error, error_message.c_str());

        return EXCEPTION_CONTINUE_EXECUTION;
    }
    std::unique_ptr<HMODULE, void (*)(HMODULE *)> ptr_lib_module(&hDbgHelp, [](HMODULE *p_module) {
        if (*p_module != NULL)
        {
            FreeLibrary(*p_module);
        }
    });

    pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

    if (NULL == pfnMiniDumpWriteDump)
    {
        int error = GetLastError();
        std::string error_message = std::system_category().message(error);
        printf("DbgHelp.dll GetProcAddress MiniDumpWriteDump failed,%d:%s\n", error, error_message.c_str());

        return EXCEPTION_CONTINUE_EXECUTION;
    }

    // 写入 dmp 文件
    MINIDUMP_EXCEPTION_INFORMATION expParam;
    expParam.ThreadId = GetCurrentThreadId();
    expParam.ExceptionPointers = lpExceptionInfo;
    expParam.ClientPointers = FALSE;
    bool success = pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithFullMemory,
                                        (lpExceptionInfo ? &expParam : NULL), NULL, NULL);

    if (!success)
    {
        int error = GetLastError();
        std::string error_message = std::system_category().message(error);
        printf("MiniDumpWriteDump failed,%d:%s\n", error, error_message.c_str());

        return EXCEPTION_CONTINUE_EXECUTION;
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

#endif