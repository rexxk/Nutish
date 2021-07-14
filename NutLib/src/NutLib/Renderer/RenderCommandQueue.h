#ifndef __NUTLIB_RENDERCOMMANDQUEUE_H
#define __NUTLIB_RENDERCOMMANDQUEUE_H

#include "nutpch.h"

#include "NutLib/Core/Application.h"
#include "NutLib/Event/Events.h"


namespace Nut
{

	class RenderCommandQueue
	{
	public:

		template<typename Fn>
		static void Submit(Fn fn)
		{
			std::lock_guard<std::mutex> lock(s_CommandMutex);
			s_Commands.emplace_back(fn);

//			LOG_CORE_TRACE("size: {0}", RenderCommandQueue::s_Commands.size());
		}

		static void Run()
		{
			LOG_CORE_TRACE("Starting rendering thread");

			s_Thread = std::thread(&RenderCommandQueue::RenderFunc, Application::Get().GetWindow());
		}

		static void Stop()
		{
			LOG_CORE_TRACE("Stopping rendering thread");

			s_Running = false;

		}

		static void Join()
		{
			LOG_CORE_TRACE("Joining render thread");
			s_Thread.join();
		}

		static void Execute()
		{
			std::lock_guard<std::mutex> lock(s_ExecuteMutex);
			s_Executing = true;
		}

		static bool Idle()
		{
			return !s_Executing;
		}

		static void IncreaseFPS()
		{
			std::lock_guard<std::mutex> lock(s_FpsMutex);
			s_FPS++;
		}

		static void ResetFPS()
		{
			std::lock_guard<std::mutex> lock(s_FpsMutex);
			s_FPS = 0;
		}

		static uint32_t FPS()
		{
			std::lock_guard<std::mutex> lock(s_FpsMutex);
			return s_FPS;
		}

		static void ClearBuffer()
		{
			LOG_CORE_ERROR("Clearing buffer");
			std::lock_guard<std::mutex> lock(s_CommandMutex);
			RenderCommandQueue::s_Commands.clear();
		}

		static void Present()
		{
			std::lock_guard<std::mutex> lock(s_PresentMutex);
			s_Present = true;
		}

		static void RenderFunc(Ref<Window> window)
		{
			window->GetRenderContext()->Bind();
			
#if _WIN32
			HDC deviceContext = GetDC(static_cast<HWND>(window->GetNativeHandle()));
#endif


			while (s_Running)
			{
				if (s_Executing)
				{
//					std::lock_guard<std::mutex> lock(s_CommandMutex);
					s_CommandMutex.lock();

					auto commands = s_Commands;

					s_Commands.clear();
					s_CommandMutex.unlock();

//					RenderCommandQueue::ClearBuffer();

//					LOG_CORE_TRACE("Buffer size: {0}", commands.size());

					for (auto& command : commands)
					{
						command();
					}

					std::lock_guard<std::mutex> lock(s_ExecuteMutex);
					s_Executing = false;
				}

				if (s_Present)
				{
#if _WIN32
					SwapBuffers(deviceContext);
#endif

					std::lock_guard<std::mutex> presentLock(s_PresentMutex);
					s_Present = false;

					IncreaseFPS();
				}

			}
		}

	public:
		static inline std::vector<std::function<void()>> s_Commands;

	private:
		static inline std::thread s_Thread;
		static inline std::mutex s_CommandMutex;
		static inline std::mutex s_ExecuteMutex;
		static inline std::mutex s_FpsMutex;
		static inline std::mutex s_PresentMutex;

		static inline std::atomic<bool> s_Running = true;
		static inline std::atomic<bool> s_Executing = false;
		static inline std::atomic<bool> s_Present = false;

		static inline uint32_t s_FPS = 0;
	};

}



#endif
