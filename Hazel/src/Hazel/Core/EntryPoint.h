#pragma once
#include "Hazel/Core/Base.h"
#include "Hazel/Core/Application.h"

#ifdef HZ_PLATFORM_WINDOWS
//声明Creat函数
extern Hazel::Application* Hazel::CreateApplication(ApplicationCommandLineArgs args);
//程序入口
int main(int argc, char** argv)
{
	Hazel::Log::Init();

	HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
	//该函数的作用是根据传入的命令行参数创建一个Hazel应用程序，并返回指向该应用程序的指针。
	auto app = Hazel::CreateApplication({ argc, argv });
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
	delete app;
	HZ_PROFILE_END_SESSION();
}

#endif
