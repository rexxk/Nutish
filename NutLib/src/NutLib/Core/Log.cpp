#include "nutpch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>



namespace Nut
{

	void Log::Init()
	{
		s_CoreLogger = spdlog::stdout_color_mt("NutLib");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->set_pattern("%^[%T]: %v%$");

		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_pattern("%^[%n]: %v%$");
	}


}
