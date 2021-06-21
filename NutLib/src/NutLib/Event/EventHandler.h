#pragma once

#include "Event.h"


namespace Nut
{

	template<typename T>
	class EventCallback
	{
	public:
		template<typename Fn>
		static void AddCallback(Fn fn)
		{
			if (!s_Instance)
				s_Instance = new EventCallback<T>();

			s_Instance->m_Callbacks.emplace_back(fn);
		}

		static size_t Size() { return s_Instance ? s_Instance->m_Callbacks.size() : 0; }

		static bool Execute(Ref<T> event)
		{
			if (s_Instance)
			{
				for (auto& callback : s_Instance->m_Callbacks)
				{
					event->Handled = callback(*(T*&)event);

					if (event->Handled)
						return true;
				}
			}

			return false;
		}

	private:
		static inline EventCallback<T>* s_Instance = nullptr;

		std::vector<std::function<bool(T&)>> m_Callbacks;
	};


	class EventQueue
	{
	public:
		void AddEvent(Ref<Event> event)
		{
			m_Events.emplace(event);
		}

		Ref<Event> GetEvent()
		{
			auto event = m_Events.front();
			m_Events.pop();

			return event;
		}

		size_t Size() const { return m_Events.size(); }

	private:
		std::queue<Ref<Event>> m_Events;
	};

	template<typename T, typename Fn>
	void SubscribeToEvent(Fn fn)
	{
		EventCallback<T>::AddCallback(fn);
	}

	template<typename T, typename ... Args>
	Ref<T> MakeEvent(Args ... args)
	{
		Ref<T> event = CreateRef<T>(std::forward<Args>(args)...);
		return event;
	}

	void AddEvent(Ref<Event> event);
	void ExecuteEvent(Ref<Event> event);
	void ProcessEvent(Ref<Event> event);

}
