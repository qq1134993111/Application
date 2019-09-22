#pragma once
#include"GeneralApplication.h"

class MyApp :public general::GeneralApplication
{
public:
	MyApp();
	~MyApp();
private:
	virtual void SetProgramOption();
	virtual void OnProgramOption(const std::string& option_name) {}
	virtual int32_t OnParseProgramOptionEnd() { return static_cast<int32_t>(ErrorCode::kSuccess); }
	virtual int32_t OnLogInit(general::Property& log_prop);
	virtual int32_t OnInit() { return static_cast<int32_t>(ErrorCode::kSuccess); }
	virtual int32_t OnRun() { return static_cast<int32_t>(ErrorCode::kPassed); }
	virtual void OnIdle(){};
	virtual void OnSignal(int sig_num, int value) {}
	virtual void OnExit() {}
	std::string  config_path_;
};


