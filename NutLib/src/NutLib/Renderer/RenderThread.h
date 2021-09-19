#ifndef __NUTLIB_RENDERCOMMANDQUEUE_H
#define __NUTLIB_RENDERCOMMANDQUEUE_H

#include "nutpch.h"

#include "NutLib/Core/Application.h"
#include "NutLib/Event/Events.h"


namespace Nut
{

	class RenderThread
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
				s_Instance = new RenderThread();

				NUT_CORE_ASSERT(s_Instance, "Failed to create render command queue");
			}
		}

		static void Shutdown()
		{
			if (s_Instance)
			{
				delete s_Instance;

				s_Instance = nullptr;
			}
		}


		RenderThread()
		{

		}

		~RenderThread()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");

			if (s_Instance->m_Thread)
			{
				delete m_Thread;
				m_Thread = nullptr;
			}
		}

		template<typename Fn>
		static void Submit(Fn fn)
		{
			if (s_Instance)
			{
				if (IsRunning())
				{
					std::lock_guard<std::mutex> lock(s_Instance->m_CommandMutex);
					s_Instance->m_CommandQueue.push(fn);
				}
			}
		}

		static void Run()
		{
			LOG_CORE_TRACE("Starting rendering thread");

			auto& window = Application::Get().GetWindow();

			s_Instance->m_Thread = new std::thread([&]()
				{
					s_Instance->m_ThreadFinished.store(false);

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
									s_Instance->m_Executing.store(true);

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

									s_Instance->m_Executing.store(false);

									break;
								}

								case QueueCommand::Present:
								{
									s_Instance->m_Present.store(true);

	#if _WIN32
									SwapBuffers(deviceContext);
	#endif

									{
										std::lock_guard<std::mutex> lock(s_Instance->m_FpsMutex);
										s_Instance->m_FPS++;
									}

									s_Instance->m_Present.store(false);
									s_Instance->m_FrameDone.store(true);

									break;
								}

								case QueueCommand::Shutdown:
								{
									s_Instance->m_Running.store(false);

									break;
								}
							}
						}


					}

					s_Instance->m_ThreadFinished.store(true);

				});


			NUT_CORE_ASSERT(s_Instance->m_Thread, "Unable to create render thread");

		}

		static void Stop()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");

			{
				std::lock_guard<std::mutex> lock(s_Instance->m_QueueCommandsMutex);
				s_Instance->m_QueueCommands.push(QueueCommand::Shutdown);
			}

			while (!s_Instance->m_ThreadFinished)
			{

			}

			while (!s_Instance->m_Thread->joinable())
			{
				LOG_CORE_TRACE("RenderThread - waiting to join");
			}

			s_Instance->m_Thread->join();
			s_Instance->m_ThreadStopped = true;

			LOG_CORE_TRACE("Render thread stopped");
		}

		static bool ThreadStopped()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");

			return s_Instance->m_ThreadStopped;
		}

		static void Execute()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");

			s_Instance->m_FrameDone.store(false);

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

		static bool Idle()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");
			
			return !(s_Instance->m_Executing || s_Instance->m_Present);
		}

		static bool IsFrameDone()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");
			
			return s_Instance->m_FrameDone.load();
		}

		static void ResetFPS()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");

			{
				std::lock_guard<std::mutex> lock(s_Instance->m_FpsMutex);
				s_Instance->m_FPS = 0;
			}
		}

		static uint32_t FPS()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");

			{
				std::lock_guard<std::mutex> lock(s_Instance->m_FpsMutex);
				return s_Instance->m_FPS;
			}
		}

		static void Present()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");

			{
				std::lock_guard<std::mutex> lock(s_Instance->m_QueueCommandsMutex);
				s_Instance->m_QueueCommands.push(QueueCommand::Present);
			}
		}

		static bool IsAlive()
		{
			NUT_CORE_ASSERT(s_Instance, "No valid instance");

			return !s_Instance->m_ThreadStopped;
		}

		static bool IsRunning()
		{
			if (s_Instance->m_Running.load())
				return true;

			return false;
		}

	private:
		std::queue<std::function<void()>> m_CommandQueue;
		std::queue<std::function<void()>> m_ExecQueue;

		static inline RenderThread* s_Instance = nullptr;

	private:
		std::thread* m_Thread = nullptr;

		std::mutex m_CommandMutex;
		std::mutex m_ExecQueueMutex;
		std::mutex m_FpsMutex;
		std::mutex m_QueueCommandsMutex;

		std::atomic<bool> m_Executing = false;
		std::atomic<bool> m_FrameDone = false;
		std::atomic<bool> m_Present = false;
		std::atomic<bool> m_Running = true;

		std::atomic<bool> m_ThreadFinished = false;
		std::atomic<bool> m_ThreadStopped = false;

		uint32_t m_FPS = 0;

		std::queue<QueueCommand> m_QueueCommands;
	};

}



#endif
