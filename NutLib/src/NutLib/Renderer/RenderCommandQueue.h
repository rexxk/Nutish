#pragma once

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

		static void Execute(bool present = false)
		{
			std::lock_guard<std::mutex> lock(s_UpdateMutex);
			s_Update = true;

			s_Present = present;
		}

		static bool Idle()
		{
			return !s_Update;
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
			return s_FPS;
		}

		static void ClearBuffer()
		{
//			LOG_CORE_ERROR("Clearing buffer");
			std::lock_guard<std::mutex> lock(s_CommandMutex);
			RenderCommandQueue::s_Commands.clear();
		}

		static void RenderFunc(Ref<Window> window)
		{
//			Application::Get().GetWindow()->GetRenderContext()->Bind();
			window->GetRenderContext()->Bind();


			while (s_Running)
			{
				if (s_Update)
				{
//					std::lock_guard<std::mutex> lock(s_CommandMutex);
					s_CommandMutex.lock();

					for (auto& command : s_Commands)
					{
						command();
					}
					
					s_CommandMutex.unlock();


					RenderCommandQueue::ClearBuffer();

					std::lock_guard<std::mutex> updateLock(s_UpdateMutex);
					s_Update = false;

				}

				if (s_Present)
				{
//					AddEvent(MakeEvent<WindowRenderEvent>());
					window->Present();

					std::lock_guard<std::mutex> updateLock(s_UpdateMutex);
					s_Present = false;
				}

			}

			wglMakeCurrent(NULL, NULL);
		}

	public:
		static inline std::vector<std::function<void()>> s_Commands;

	private:
		static inline std::thread s_Thread;
		static inline std::mutex s_CommandMutex;
		static inline std::mutex s_UpdateMutex;
		static inline std::mutex s_FpsMutex;

		static inline std::atomic<bool> s_Running = true;
		static inline std::atomic<bool> s_Update = false;
		static inline std::atomic<bool> s_Present = false;

		static inline uint32_t s_FPS = 0;
	};

}
