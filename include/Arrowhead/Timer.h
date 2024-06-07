#pragma once

#include <iostream>
#include <chrono>
#include <string>

// Returning const char pointer for speed and simplicity
// Is this overcomplicated?
template<typename duration_type>
const inline char* getDurationName();

template<>
const inline char* getDurationName<std::chrono::nanoseconds>()
{
	return "nanoseconds";
}

template<>
const inline char* getDurationName<std::chrono::microseconds>()
{
	return "microseconds";
}

template<>
const inline char* getDurationName<std::chrono::milliseconds>()
{
	return "milliseconds";
}

template<>
const inline char* getDurationName<std::chrono::seconds>()
{
	return "seconds";
}

template<>
const inline char* getDurationName<std::chrono::minutes>()
{
	return "minutes";
}

template<>
const inline char* getDurationName<std::chrono::hours>()
{
	return "hours";
}

template<typename duration_type>
class Timer
{
public:
	Timer(std::string name)
		: m_Start(std::chrono::steady_clock::now()), m_Name(name), m_Print(true) {}

	Timer(const char* name)
		: m_Start(std::chrono::steady_clock::now()), m_Name(name), m_Print(true) {}

	Timer(std::string name, bool print)
		: m_Start(std::chrono::steady_clock::now()), m_Name(name), m_Print(print) {}

	Timer(const char* name, bool print)
		: m_Start(std::chrono::steady_clock::now()), m_Name(name), m_Print(print) {}

	~Timer()
	{
		if (!m_Triggered)
			Trigger();
	}

	uint64_t Trigger()
	{
		m_Triggered = true;
		m_Stop = std::chrono::steady_clock::now();
		uint64_t finalTime = std::chrono::duration_cast<duration_type>(m_Stop - m_Start).count();
		if (m_Print)
			std::cout << '[' << m_Name << "] exited with a final time of " << finalTime <<
				' ' << getDurationName<duration_type>() << ".\n";

		return finalTime;
	}

private:
	std::chrono::steady_clock::time_point m_Start;
	std::chrono::steady_clock::time_point m_Stop;
	bool m_Triggered = false;
	bool m_Print;

	std::string m_Name;
};