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
			Idle,
			Execute,
			Present,
			Shutdown,
		};

	public:

		static void Init()
		{
			if (s_Instance == nullptr)
			{
				s_Instance = new RenderCommandQueue();

				NUT_CORE_ASSERT(s_Instance, "Failed to create render command queue");
			}

//			s_Instance->Run();
		}

/*		static void Shutdown()
		{
			if (s_Instance)
			{
				delete s_Instance;

				s_Instance = nullptr;
			}
		}
*/

		RenderCommandQueue()
		{

		}

		~RenderCommandQueue()
		{
			LOG_CORE_TRACE("RenderCommandQueue destructor called");

			if (s_Instance->m_Thread)
			{
				LOG_CORE_TRACE("RenderCommandQueue destructor wants to delete the m_Thread object");

				delete m_Thread;
				m_Thread = nullptr;
			}
		}

		template<typename Fn>
		static void Submit(Fn fn)
		{
			if (s_Instance)
			{
				if (s_Instance->m_Running)
				{
					std::lock_guard<std::mutex> lock(s_Instance->m_CommandMutex);
					s_Instance->m_CommandQueue.push(fn);
				}
				else
				{
					LOG_CORE_TRACE("RenderThread::Submit - no valid thread to submit to");
				}
			}
		}

		static void Run()
		{
			LOG_CORE_TRACE("Starting rendering thread");

			auto& window = Application::Get().GetWindow();

			s_Instance->m_Thread = new std::thread([&]()
				{
					s_Instance->m_ThreadFinished = false;

					window->GetRenderContext()->Bind();

#if _WIN32
					HDC deviceContext = GetDC(static_cast<HWND>(window->GetNativeHandle()));
#endif
					while (s_Instance->m_Running)
					{
						if (!s_Instance->m_QueueCommands.empty())
						{
							QueueCommand queueCommand = QueueCommand::Idle;

							{
								std::lock_guard<std::mutex> lock(s_Instance->m_QueueCommandsMutex);
								queueCommand = s_Instance->m_QueueCommands.front();
								s_Instance->m_QueueCommands.pop();
							}

							switch (queueCommand)
							{

								case QueueCommand::Execute:
								{
									{
										std::lock_guard<std::mutex> lock(s_Instance->m_ExecuteMutex);
										s_Instance->m_Executing = true;
									}

									auto& execQueue = s_Instance->m_ExecQueue;

									while (!execQueue.empty())
									{
										auto command = execQueue.front();

										{
											std::lock_guard<std::mutex> lock(s_Instance->m_ExecQueueMutex);
											execQueue.pop();
										}

										command();
									}

									{
										std::lock_guard<std::mutex> lock(s_Instance->m_ExecuteMutex);
										s_Instance->m_Executing = false;
									}

									break;
								}

								case QueueCommand::Present:
								{
									{
										std::lock_guard<std::mutex> lock(s_Instance->m_PresentMutex);
										s_Instance->m_Present = true;
									}

	#if _WIN32
									SwapBuffers(deviceContext);
	#endif

									{
										std::lock_guard<std::mutex> lock(s_Instance->m_FpsMutex);
										s_Instance->m_FPS++;
									}

	//								if (s_Instance->m_Present && !s_Instance->m_Executing)
	//								{
	//								}
	//								else
	//									LOG_CORE_WARN("Present and execute, not valid!");

									{
										std::lock_guard<std::mutex> lock(s_Instance->m_PresentMutex);
										s_Instance->m_Present = false;
									}

									{
										std::lock_guard<std::mutex> lock(s_Instance->m_FrameDoneMutex);
										s_Instance->m_FrameDone = true;
									}

									break;
								}

								case QueueCommand::Shutdown:
								{
									{
										std::lock_guard<std::mutex> lock(s_Instance->m_RunningMutex);
										s_Instance->m_Running = false;
									}

									{
										std::lock_guard<std::mutex> lock(s_Instance->m_CommandMutex);

										while (s_Instance->m_CommandQueue.size() > 0)
										{
											auto command = s_Instance->m_CommandQueue.front();
											s_Instance->m_CommandQueue.pop();

											command();
										}

									}

									break;
								}
							}
						}


					}

					{
						std::lock_guard<std::mutex> lock(s_Instance->m_FinishedMutex);
						s_Instance->m_ThreadFinished = true;

						//				LOG_CORE_WARN("Command queue size: {0}", s_CommandQueue.size());
						//				LOG_CORE_WARN("Queue command size: {0}", s_QueueCommands.size());

						//				LOG_CORE_TRACE("RenderThread stopped running");
					}

				});

			//			s_Instance->m_Thread = new std::thread(&RenderCommandQueue::RenderFunc, Application::Get().GetWindow());
/*			s_Instance->m_Thread = new std::thread([&](Window* window)
 {



					}, Application::Get().GetWindow());
*/
//			LOG_CORE_TRACE("RenderCommandQueue loop starting");

			NUT_CORE_ASSERT(s_Instance->m_Thread, "Unable to create render thread");

			}

		static void Stop()
		{
			if (s_Instance)
			{

				{
					std::lock_guard<std::mutex> lock(s_Instance->m_QueueCommandsMutex);
					s_Instance->m_QueueCommands.push(QueueCommand::Shutdown);
				}

				while (!s_Instance->m_ThreadFinished)
				{

				}

				LOG_CORE_TRACE("Stopping rendering thread");

				while (!s_Instance->m_Thread->joinable())
				{
					LOG_CORE_TRACE("RenderThread - waiting to join");
				}

				LOG_CORE_TRACE("RenderThread is joinable, joining");
				s_Instance->m_Thread->join();

				s_Instance->m_ThreadStopped = true;

			}


		}

		static bool ThreadStopped()
		{
			if (s_Instance)
			{
				return s_Instance->m_ThreadStopped;
			}

			return true;
		}

//		static void Join()
//		{
//			s_Instance->m_Thread->join();

//			LOG_CORE_TRACE("Joining render thread");
//		}

		static void Execute()
		{
			if (s_Instance)
			{

				{
					std::lock_guard<std::mutex> lock(s_Instance->m_FrameDoneMutex);
					s_Instance->m_FrameDone = false;
				}

				{
					std::lock_guard<std::mutex> execLock(s_Instance->m_ExecQueueMutex);
					std::lock_guard<std::mutex> commandLock(s_Instance->m_CommandMutex);

					s_Instance->m_ExecQueue.swap(s_Instance->m_CommandQueue);
				}

				{
					std::lock_guard<std::mutex> queueLock(s_Instance->m_QueueCommandsMutex);
					s_Instance->m_QueueCommands.push(QueueCommand::Execute);
				}

			}
			else
			{
				LOG_CORE_WARN("RenderThread::Execute - no valid instance");
			}
		}

		static bool Idle()
		{
			if (s_Instance)
				return !(s_Instance->m_Executing || s_Instance->m_Present);
			else
			{
				LOG_CORE_WARN("RenderThread::Idle - no valid instance");
			}

			return true;
		}

		static bool IsFrameDone()
		{
			if (s_Instance)
				return s_Instance->m_FrameDone;
			else
			{
				LOG_CORE_WARN("RenderThread::IsFrameDone - no valid instance");
			}

			return true;
		}

		static void ResetFPS()
		{
			if (s_Instance)
			{
				std::lock_guard<std::mutex> lock(s_Instance->m_FpsMutex);
				s_Instance->m_FPS = 0;
			}
			else
			{
				LOG_CORE_WARN("RenderThread::ResetFPS - no valid instance");
			}
		}

		static uint32_t FPS()
		{
			if (s_Instance)
			{
//				std::lock_guard<std::mutex> lock(s_FpsMutex);
				return s_Instance->m_FPS;
			}
			else
			{
				LOG_CORE_WARN("RenderThread::FPS - no valid instance");
			}

			return 0;
		}

		static void Present()
		{
			if (s_Instance)
			{
				std::lock_guard<std::mutex> lock(s_Instance->m_QueueCommandsMutex);
				s_Instance->m_QueueCommands.push(QueueCommand::Present);
			}
			else
			{
				LOG_CORE_WARN("RenderThread::Present - no valid instance");
			}
		}
/*
		static void RenderFunc(Ref<Window> window)
		{
			s_Instance->m_ThreadFinished = false;

			window->GetRenderContext()->Bind();

#if _WIN32
			HDC deviceContext = GetDC(static_cast<HWND>(window->GetNativeHandle()));
#endif

//			LOG_CORE_TRACE("RenderCommandQueue loop starting");

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
							{
								std::lock_guard<std::mutex> lock(s_ExecuteMutex);
								s_Executing = true;
							}

							auto& execQueue = s_ExecQueue;

							while (!execQueue.empty())
							{
								auto command = execQueue.front();

								{
									std::lock_guard<std::mutex> lock(s_ExecQueueMutex);
									execQueue.pop();
								}

								command();
							}
							
							{
								std::lock_guard<std::mutex> lock(s_ExecuteMutex);
								s_Executing = false;
							}

							break;
						}

						case QueueCommand::Present:
						{
							{
								std::lock_guard<std::mutex> lock(s_PresentMutex);
								s_Present = true;
							}

							if (s_Present && !s_Executing)
							{
#if _WIN32
								SwapBuffers(deviceContext);
#endif

								{
									std::lock_guard<std::mutex> lock(s_Instance->m_FpsMutex);
									s_Instance->m_FPS++;
								}
							}
							else
								LOG_CORE_WARN("Present and execute, not valid!");

							{
								std::lock_guard<std::mutex> lock(s_PresentMutex);
								s_Present = false;
							}

							{
								std::lock_guard<std::mutex> lock(s_FrameDoneMutex);
								s_FrameDone = true;
							}

							break;
						}

					}
				}

			}

			{

//				LOG_CORE_WARN("Command queue size: {0}", s_CommandQueue.size());
//				LOG_CORE_WARN("Queue command size: {0}", s_QueueCommands.size());

//				LOG_CORE_TRACE("RenderThread stopped running");

				std::lock_guard<std::mutex> lock(s_FinishedMutex);
				s_ThreadFinished = true;
			}

		}
*/

	private:
		std::queue<std::function<void()>> m_CommandQueue;
		std::queue<std::function<void()>> m_ExecQueue;

		static inline RenderCommandQueue* s_Instance = nullptr;

	private:
		std::thread* m_Thread = nullptr;

		std::mutex m_CommandMutex;
		std::mutex m_ExecuteMutex;
		std::mutex m_ExecQueueMutex;
		std::mutex m_FinishedMutex;
		std::mutex m_FpsMutex;
		std::mutex m_FrameDoneMutex;
		std::mutex m_QueueCommandsMutex;
		std::mutex m_PresentMutex;
		std::mutex m_RunningMutex;

		std::atomic<bool> m_Running = true;
		std::atomic<bool> m_Executing = false;
		std::atomic<bool> m_Present = false;
		std::atomic<bool> m_FrameDone = false;

		std::atomic<bool> m_ThreadFinished = false;

		std::atomic<bool> m_ThreadStopped = false;

		uint32_t m_FPS = 0;

		std::queue<QueueCommand> m_QueueCommands;
	};

}



#endif
