#ifndef __NUTLIB_RENDERCOMMANDQUEUE_H
#define __NUTLIB_RENDERCOMMANDQUEUE_H

#include "nutpch.h"

#include "NutLib/Core/Application.h"
#include "NutLib/Event/Events.h"


namespace Nut
{

	class RenderCommandQueue
	{
		enum class QueueCommand
		{
			Execute,
			Present,
		};

	public:

		template<typename Fn>
		static void Submit(Fn fn)
		{
			{
				std::lock_guard<std::mutex> lock(s_CommandMutex);
				s_CommandQueue.push(fn);
			}
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

			{
				s_FrameDone = false;

				std::lock_guard<std::mutex> lock(s_CommandMutex);
				s_ExecQueue.swap(s_CommandQueue);
			}

			s_QueueCommands.push(QueueCommand::Execute);
		}

		static bool Idle()
		{
			return !(s_Executing || s_Present);
		}

		static bool IsFrameDone()
		{
			return s_FrameDone;
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

		static void Present()
		{
			std::lock_guard<std::mutex> lock(s_CommandMutex);
			s_QueueCommands.push(QueueCommand::Present);
		}

		static void RenderFunc(Ref<Window> window)
		{
			window->GetRenderContext()->Bind();

#if _WIN32
			HDC deviceContext = GetDC(static_cast<HWND>(window->GetNativeHandle()));
#endif

			while (s_Running)
			{
				if (!s_QueueCommands.empty())
				{
					QueueCommand queueCommand = s_QueueCommands.front();

					{
						std::lock_guard<std::mutex> lock(s_CommandMutex);
						s_QueueCommands.pop();
					}

					switch (queueCommand)
					{

						case QueueCommand::Execute:
						{
							s_Executing = true;

							auto& execQueue = s_ExecQueue;

							while (!execQueue.empty())
							{
								auto command = execQueue.front();
								execQueue.pop();

								command();
							}
							
							s_Executing = false;

							break;
						}

						case QueueCommand::Present:
						{
							s_Present = true;

							if (s_Present && !s_Executing)
							{
#if _WIN32
								SwapBuffers(deviceContext);
#endif

								IncreaseFPS();
							}
							else
								LOG_CORE_WARN("Present and execute, not valid!");

							s_Present = false;

							s_FrameDone = true;

							break;
						}

					}
				}

			}
	
		}

	public:
		static inline std::queue<std::function<void()>> s_CommandQueue;

	private:
		static inline std::queue<std::function<void()>> s_ExecQueue;

	private:
		static inline std::thread s_Thread;
		static inline std::mutex s_CommandMutex;
		static inline std::mutex s_ExecuteMutex;
		static inline std::mutex s_FpsMutex;
		static inline std::mutex s_PresentMutex;

		static inline std::atomic<bool> s_Running = true;
		static inline std::atomic<bool> s_Executing = false;
		static inline std::atomic<bool> s_Present = false;
		static inline std::atomic<bool> s_FrameDone = false;

		static inline uint32_t s_FPS = 0;

		static inline std::queue<QueueCommand> s_QueueCommands;
	};

}



#endif
