// Application.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include "GeneralApplication.h"
#include <iostream>
#include "GeneralLog.h"
using namespace general;
int main(int argc, char* argv[])
{
	std::cout << C_SYSTEM << std::endl;
	//应用程序
	GeneralApplication app;
	app.Run(argc, argv);
	return 0;
}

