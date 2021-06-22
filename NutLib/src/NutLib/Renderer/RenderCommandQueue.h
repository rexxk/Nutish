#pragma once

#include "nutpch.h"



namespace Nut
{

	class RenderCommandQueue
	{
	public:

		template<typename Fn>
		static void Submit(Fn fn)
		{
			std::lock_guard<std::mutex> lock(s_Mutex);

			s_Commands.emplace_back(fn);
		}

		static void Execute()
		{

/*			for (auto& command : s_Commands)
			{
				command();
			}
			s_Commands.clear();
*/
			std::thread renderThread(&RenderCommandQueue::RenderFunc);

			renderThread.detach();
		}

		static void RenderFunc()
		{

			std::lock_guard<std::mutex> lock(s_Mutex);

			for (auto& command : s_Commands)
			{
				command();
			}


/*			s_Mutex.lock();

			auto commands = s_Commands;
			s_Commands.clear();

			s_Mutex.unlock();

			for (auto& command : commands)
			{
				command();
			}
*/

		}

	private:
		static inline std::vector<std::function<void()>> s_Commands;

		static inline std::mutex s_Mutex;
	};

}
