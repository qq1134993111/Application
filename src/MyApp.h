#pragma once
#include "GeneralApplication.h"

class MyApp : public general::GeneralApplication
{
public:
    MyApp();
    ~MyApp() override;

private:
    void SetProgramOption() override;
    void OnProgramOption(const std::string& option_name) override;
    int32_t OnParseProgramOptionEnd() override { return static_cast<int32_t>(ErrorCode::kSuccess); }
    int32_t OnLogInit(general::Property& log_prop) override;
    int32_t OnInit() override;
    int32_t OnStart() override;
    int32_t OnRun() override;

    void OnIdle() override
    {
    };
    //virtual void OnSignal(int sig_num, int value) {}
    void OnExit() override
    {
    }

    std::string config_path_;
};
