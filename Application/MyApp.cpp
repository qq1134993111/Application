#include "MyApp.h"



MyApp::MyApp()
{
}


MyApp::~MyApp()
{
}

void MyApp::SetProgramOption()
{
	std::string desc = "config path,default exe dir," + GetExeFilePath();
	AddOptionWithArgument<std::string>("config_path,c", desc.c_str(), GetExeFilePath(), &config_path_);
}

void MyApp::OnProgramOption(const std::string &option_name)
{
}

int32_t MyApp::OnLogInit(general::Property& log_prop)
{
	log_prop.SetValue(general::log_config_key::kUseConsoleLogger, true);
	log_prop(general::log_config_key::kAsyncMode, true);
	log_prop.SetValue(general::log_config_key::kLoggerType, general::log_config_key::LoggerType::kLoggerTypeDaily);
	return static_cast<int32_t>(ErrorCode::kSuccess);
}

int32_t MyApp::OnInit()
{
    return static_cast<int32_t>(ErrorCode::kSuccess);
}

int32_t MyApp::OnStart()
{
    return static_cast<int32_t>(ErrorCode::kSuccess);
}

int32_t MyApp::OnRun()
{
	// return static_cast<int32_t>(ErrorCode::kPassed);
    return static_cast<int32_t>(ErrorCode::kPassed);
}
