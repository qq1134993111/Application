// Application.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include "GeneralApplication.h"

#include "GeneralLog.h"
using namespace general;
int main(int argc,char* argv[])
{
	
	GeneralApplication app;
	app.Run(argc, argv);
    return 0;
}

