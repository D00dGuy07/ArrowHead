#pragma once

#include <functional>

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