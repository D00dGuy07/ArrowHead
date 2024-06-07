#pragma once

#include <vector>
#include <tuple>
#include <functional>

#include "oglfv2/Util/Events/EventConnection.h"
#include "oglfv2/Util/Events/CallbackList.h"

template<typename... arguments> class EventQueue;

template<typename... arguments>
class EventQueue
{
public:
	std::shared_ptr<EventConnection<arguments...>> Connect(std::function<void(arguments...)> callback)
	{ return m_CallbackList.Connect(callback); }

	void Enqueue(arguments... args)
	{
		m_Queue.push_back(std::tuple<arguments...>(args...));
	}

	void Process()
	{
		// Switching to use callback list is way faster than I thought
		// I guess not calling std::apply every time saves a lot
		for (auto& event : m_Queue)
			std::apply([&](arguments... args) {
				m_CallbackList.Call(args...);
			}, event);
	}

private:
	CallbackList<arguments...> m_CallbackList;
	std::vector<std::tuple<arguments...>> m_Queue;
};