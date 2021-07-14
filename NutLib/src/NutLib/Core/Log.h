#ifndef __NUTLIB_LOG_H
#define __NUTLIB_LOG_H

#include <spdlog/spdlog.h>


namespace Nut
{


	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static inline Ref<spdlog::logger> s_CoreLogger = nullptr;
		static inline Ref<spdlog::logger> s_ClientLogger = nullptr;
	};


}


#ifdef _WIN32

	#define LOG_CORE_TRACE(x, ...) Nut::Log::GetCoreLogger()->trace(x, __VA_ARGS__);
	#define LOG_CORE_WARN(x, ...) Nut::Log::GetCoreLogger()->warn(x, __VA_ARGS__);
	#define LOG_CORE_ERROR(x, ...) Nut::Log::GetCoreLogger()->error(x, __VA_ARGS__);
	#define LOG_CORE_CRITICAL(x, ...) Nut::Log::GetCoreLogger()->critical(x, __VA_ARGS__);

	#define LOG_TRACE(x, ...) Nut::Log::GetClientLogger()->trace(x, __VA_ARGS__);
	#define LOG_WARN(x, ...) Nut::Log::GetClientLogger()->warn(x, __VA_ARGS__);
	#define LOG_ERROR(x, ...) Nut::Log::GetClientLogger()->error(x, __VA_ARGS__);
	#define LOG_CRITICAL(x, ...) Nut::Log::GetClientLogger()->critical(x, __VA_ARGS__);

	#define NUT_CORE_ASSERT(condition, x, ...) if (!condition) { LOG_CORE_ERROR(x, __VA_ARGS__); __debugbreak(); }
	#define NUT_ASSERT(condition, x, ...) if (!condition) { LOG_ERROR(x, __VA_ARGS__); __debugbreak(); }

#elif __linux__

	#define LOG_CORE_TRACE(x, ...) Nut::Log::GetCoreLogger()->trace(x, ##__VA_ARGS__);
	#define LOG_CORE_WARN(x, ...) Nut::Log::GetCoreLogger()->warn(x, ##__VA_ARGS__);
	#define LOG_CORE_ERROR(x, ...) Nut::Log::GetCoreLogger()->error(x, ##__VA_ARGS__);
	#define LOG_CORE_CRITICAL(x, ...) Nut::Log::GetCoreLogger()->critical(x, ##__VA_ARGS__);

	#define LOG_TRACE(x, ...) Nut::Log::GetClientLogger()->trace(x, ##__VA_ARGS__);
	#define LOG_WARN(x, ...) Nut::Log::GetClientLogger()->warn(x, ##__VA_ARGS__);
	#define LOG_ERROR(x, ...) Nut::Log::GetClientLogger()->error(x, ##__VA_ARGS__);
	#define LOG_CRITICAL(x, ...) Nut::Log::GetClientLogger()->critical(x, ##__VA_ARGS__);

	#define NUT_CORE_ASSERT(condition, x, ...) if (!condition) { LOG_CORE_ERROR(x, ##__VA_ARGS__); }
	#define NUT_ASSERT(condition, x, ...) if (!condition) { LOG_ERROR(x, ##__VA_ARGS__); }

#endif


#endif
