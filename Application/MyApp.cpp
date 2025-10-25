#include "MyApp.h"

#include "net/TcpClient.h"


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
	//log_prop.SetValue(general::log_config_key::kUseConsoleLogger, true);
	//log_prop(general::log_config_key::kAsyncMode, true);
	//log_prop.SetValue(general::log_config_key::kLoggerType, general::log_config_key::LoggerType::kLoggerTypeDaily);
	return static_cast<int32_t>(ErrorCode::kSuccess);
}



int32_t MyApp::OnInit()
{


    std::thread([this]() {
		auto guard=net::make_work_guard(ios_);
		ios_.run();
        std::cout << "run end\n";
		}).detach();
    
	client_.BindConnect([]() {
		std::cout << "connect\n";
		});
    client_.BindConnectFailed([]() {
        std::cout << "ConnectFailed"<< "\n";
		});
    client_.BindRecv([](std::string_view data) { 
		std::cout << "recv " << data.size() << "bytes\n";
		});
    client_.BindDisconnect([](boost::system::error_code& ec) { 
		std::cout <<ec.message()<<" disconnect\n";
        int i;
        i = 0;
		});

	client_.SetConnectTimeout(5);
    client_.SetReconnectIntervals(5);
    client_.SetHeartbeatIntervals(30, []() -> std::string {
		return "test";
		});
	client_.Connect("127.0.0.1",8088);

	while (!client_.IsConnected())
	{
        std::this_thread::sleep_for(std::chrono::seconds(1));
	}
    return static_cast<int32_t>(ErrorCode::kSuccess);
}

int32_t MyApp::OnStart()
{
  
    client_.AsyncSend("helloworld");
    client_.AsyncSend("helloworld");
    //client_.AsyncSend("helloworld");
    return static_cast<int32_t>(ErrorCode::kSuccess);
}

int32_t MyApp::OnRun()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
	//return static_cast<int32_t>(ErrorCode::kPassed);
    return static_cast<int32_t>(ErrorCode::kSuccess);
}
