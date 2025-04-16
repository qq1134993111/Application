// Application.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include "MyApp.h"
using namespace general;
int main(int argc, char* argv[])
{
	std::cout << C_SYSTEM << std::endl;
	//应用程序
	MyApp app;
	app.Run(argc, argv);
	return 0;
}

