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
//			while (m_Running)
			while (IsRunning())
			{
				Update();
			}

			{
//				std::lock_guard<std::mutex> lock(m_FinishedMutex);
//				m_Finished = true;

				m_Finished.store(true);
			}
		}

		void Reset()
		{
			LOG_CORE_TRACE("Starting timer");

			m_StartTime = std::chrono::steady_clock::now();

			LOG_CORE_TRACE("Timer start: setting timeouttime");
			{
				std::lock_guard<std::mutex> lock(m_TimerMutex);
				m_TimeoutTime = m_StartTime;
			}

			LOG_CORE_TRACE("Timer start: setting running state");
			{
//				std::lock_guard<std::mutex> lock(m_RunningMutex);
//				m_Running = true;
				m_Running.store(true);
			}

			LOG_CORE_TRACE("Timer start: starting thread");
			m_TimerThread = new std::thread(&Timer::ThreadFunc, this);

//			m_TimerThread->detach();
		}

		void Stop()
		{
			LOG_CORE_TRACE("Timer: Stopping timer");

			{
				LOG_CORE_TRACE("Timer: setting running flag to false");
//				std::lock_guard<std::mutex> lock(m_RunningMutex);
//				m_Running = false;
				m_Running.store(false);
				LOG_CORE_TRACE("Timer: running flag set to false");

			}

			LOG_CORE_TRACE("Timer: waiting to finish");
			while (!IsFinished())
			{

			}

//			LOG_CORE_TRACE("Timer: stopped");
			while (!m_TimerThread->joinable())
			{

			}

			m_TimerThread->join();
			 
			LOG_CORE_TRACE("Timer: Joining timer");
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
		bool IsRunning()
		{
			{
//				std::lock_guard<std::mutex> lock(m_RunningMutex);

				if (m_Running.load())
					return true;
			}

			return false;
		}

		bool IsFinished()
		{
			{
//				std::lock_guard<std::mutex> lock(m_FinishedMutex);

				if (m_Finished.load())
					return true;
			}

			return false;
		}

	private:
		std::chrono::steady_clock::time_point m_StartTime;
		std::chrono::steady_clock::time_point m_TimeoutTime;
	
		std::atomic<double> m_TriggerTime = 0.0;

		UUID m_Id;

		std::thread* m_TimerThread = nullptr;
		std::mutex m_TimerMutex;

//		std::mutex m_RunningMutex;
//		std::mutex m_FinishedMutex;

		std::atomic<bool> m_Running = false;
		std::atomic<bool> m_Finished = false;
	};


}


#endif
