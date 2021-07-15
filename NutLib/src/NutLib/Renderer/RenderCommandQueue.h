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
			Clear,
			Execute,
			Present,
		};

	public:

		template<typename Fn>
		static void Submit(Fn fn)
		{
			std::lock_guard<std::mutex> lock(s_CommandMutex);
			s_CommandQueue.push(fn);
//			s_Commands.emplace_back(fn);

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
//			std::lock_guard<std::mutex> lock(s_ExecuteMutex);
//			s_Executing = true;

//			if (std::find(s_QueueCommands.begin(), s_QueueCommands.end(), QueueCommand::Execute) == s_QueueCommands.end())
//				s_QueueCommands.push_back(QueueCommand::Execute);

			s_QueueCommands.push(QueueCommand::Execute);
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
//			LOG_CORE_ERROR("Clearing buffer");
//			std::lock_guard<std::mutex> lock(s_CommandMutex);
//			RenderCommandQueue::s_Commands.clear();

//			s_QueueCommands.push_back(QueueCommand::Clear);

			s_QueueCommands.push(QueueCommand::Clear);
		}

		static void Present()
		{
//			std::lock_guard<std::mutex> lock(s_PresentMutex);
//			s_Present = true;

//			if (std::find(s_QueueCommands.begin(), s_QueueCommands.end(), QueueCommand::Present) == s_QueueCommands.end())
//				s_QueueCommands.push_back(QueueCommand::Present);

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
					s_QueueCommands.pop();

					switch (queueCommand)
					{
						case QueueCommand::Clear:
						{
//							s_Commands.clear();
//							s_CommandQueue.
							LOG_CORE_WARN("Cannot clear a queue");
							break;
						}

						case QueueCommand::Execute:
						{
//							LOG_CORE_WARN("Execute");

							s_Executing = true;

//							std::queue<std::function<void()>> commands;

//							{
//								std::lock_guard<std::mutex> lock(s_CommandMutex);
//								commands = s_Commands;
//								s_Commands.clear();
//							}

//							if (commands.size() > 0)

							while (!s_CommandQueue.empty())
							{
								auto& command = s_CommandQueue.front();
								s_CommandQueue.pop();

								command();
							}
							
/*							{
								LOG_CORE_WARN("Command size: {0}", commands.size());
								for (auto& command : commands)
								{
									command();
								}
							}
*/
							s_Executing = false;

							break;
						}

						case QueueCommand::Present:
						{
//							LOG_CORE_WARN("Present");

							s_Present = true;

//							std::lock_guard<std::mutex> presentLock(s_PresentMutex);
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

							break;
						}

					}
				}

				/*
								{
									std::lock_guard<std::mutex> lock(s_ExecuteMutex);

									if (s_Executing)
									{
										s_Executing = false;

										//					std::lock_guard<std::mutex> lock(s_CommandMutex);
										s_CommandMutex.lock();

										auto commands = s_Commands;

										s_Commands.clear();
										s_CommandMutex.unlock();

										//					RenderCommandQueue::ClearBuffer();

										//					LOG_CORE_TRACE("Buffer size: {0}", commands.size());
										if (commands.size() > 0)
										{
											for (auto& command : commands)
											{
												command();
											}
										}
									}
								}

								{
									std::lock_guard<std::mutex> presentLock(s_PresentMutex);
									if (s_Present && !s_Executing)
									{
										s_Present = false;

				#if _WIN32
										SwapBuffers(deviceContext);
				#endif

										IncreaseFPS();
									}
									else
										LOG_CORE_WARN("Present and execute, not valid!");

								}
								*/

				}
			
			}

	public:
		static inline std::queue<std::function<void()>> s_CommandQueue;

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

		static inline std::queue<QueueCommand> s_QueueCommands;
	};

}



#endif
