#ifndef __NUTLIB_TIMER_H
#define __NUTLIB_TIMER_H


#include <chrono>
#include <thread>

#include <NutLib/Event/Events.h>

#include <NutLib/Core/UUID.h>


namespace Nut
{


	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		Timer(double triggerTime)
			: m_TriggerTime(triggerTime)
		{
			Reset();
		}

		~Timer()
		{

			if (m_Running)
				Stop();
//			m_Running = false;
//			m_TimerThread->join();
		}

		void ThreadFunc()
		{
			while (m_Running)
			{
				Update();
			}

			m_Finished = true;
		}

		void Reset()
		{
			m_StartTime = std::chrono::steady_clock::now();
			m_TimeoutTime = m_StartTime;

			{
				std::lock_guard<std::mutex> lock(m_RunningMutex);
				m_Running = true;
			}

			m_TimerThread = new std::thread(&Timer::ThreadFunc, this);
		}

		void Stop()
		{
			LOG_CORE_TRACE("Timer: Stopping timer");

			{
				std::lock_guard<std::mutex> lock(m_RunningMutex);
				m_Running = false;
			}

			while (!m_Finished)
			{

			}

			if (m_TimerThread->joinable())
			{
				m_TimerThread->join();
				LOG_CORE_TRACE("Timer: Joining timer");
			}
		}

		void Update()
		{
			std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();

			double timeoutTime = (double)std::chrono::duration_cast<std::chrono::microseconds>(t - m_TimeoutTime).count() / 1000000;

			if (timeoutTime >= m_TriggerTime)
			{
				std::lock_guard<std::mutex> lock(m_TimerMutex);
				m_TimeoutTime = t;

				AddEvent(MakeEvent<TimerTimeoutEvent>(m_Id, timeoutTime));
			}
		}

		double ElapsedTime()
		{
			std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();

			double elapsedTime = (double)std::chrono::duration_cast<std::chrono::microseconds>(t - m_StartTime).count() / 1000000;

			return elapsedTime;
		}

		uint64_t Id() const { return m_Id; }

	private:
		std::chrono::steady_clock::time_point m_StartTime;
		std::chrono::steady_clock::time_point m_TimeoutTime;
	
		double m_TriggerTime = 0.0;

		UUID m_Id;

		std::thread* m_TimerThread = nullptr;
		std::mutex m_TimerMutex;

		std::mutex m_RunningMutex;

		std::atomic<bool> m_Running = false;
		std::atomic<bool> m_Finished = false;
	};


}


#endif
