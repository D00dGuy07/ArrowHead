#pragma once

#include <vector>
#include <tuple>
#include <functional>
#include <memory>

#include "oglfv2/Util/Events/EventConnection.h"

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