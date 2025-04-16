#pragma once
#include "CPreDefined.h"

#if defined(C_SYSTEM_WINDOWS)
#include <string>
class DumpHelper
{
  public:
    static bool EnableDump(std::string directory = "", std::string file_name = "");
};
#endif

