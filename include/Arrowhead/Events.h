#pragma once

#include <vector>
#include <tuple>
#include <functional>
#include <memory>

namespace arwh
{
	template<typename... parameters>
	class EventConnection
	{
	public:
		EventConnection(const std::function<void(parameters...)>& callback)
			: m_Callback(callback), m_IsConnected(true) {}

		EventConnection(const EventConnection& other)
			: m_Callback(other.m_Callback), m_IsConnected(other.m_IsConnected) {}

		inline void Disconnect() { m_IsConnected = false; }
		inline bool IsConnected() const { return m_IsConnected; }

		inline const std::function<void(parameters...)> Callback() const { return m_Callback; }

	private:
		std::function<void(parameters...)> m_Callback;
		bool m_IsConnected;
	};

	template<typename... arguments>
	class CallbackList
	{
	public:
		std::shared_ptr<EventConnection<arguments...>> Connect(std::function<void(arguments...)> callback)
		{
			m_CallbackList.emplace_back(std::make_shared<EventConnection<arguments...>>(callback));
			return *(m_CallbackList.end() - 1);
		}

		void Call(arguments... args)
		{
			std::vector<std::shared_ptr<EventConnection<arguments...>>> activeCallbacks;
			activeCallbacks.reserve(m_CallbackList.size());
			for (auto& callback : m_CallbackList)
			{
				if (callback->IsConnected())
				{
					callback->Callback()(args...);
					if (callback->IsConnected())
						activeCallbacks.push_back(callback);
				}
			}

			m_CallbackList = std::move(activeCallbacks);
		}

	private:
		std::vector<std::shared_ptr<EventConnection<arguments...>>> m_CallbackList;
	};

	template<typename... arguments>
	class EventQueue
	{
	public:
		std::shared_ptr<EventConnection<arguments...>> Connect(std::function<void(arguments...)> callback)
		{
			return m_CallbackList.Connect(callback);
		}

		void Enqueue(arguments... args)
		{
			m_Queue.push_back(std::tuple<arguments...>(args...));
		}

		void Process()
		{
			// Switching to use callback list is way faster than I thought
			// I guess not calling std::apply every time saves a lot
			for (auto& event : m_Queue)
			{
				std::apply([&](arguments... args) {
					m_CallbackList.Call(args...);
				}, event);
			}
		}

	private:
		CallbackList<arguments...> m_CallbackList;
		std::vector<std::tuple<arguments...>> m_Queue;
	};
}