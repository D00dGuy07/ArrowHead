#include "Arrow/Core/Logger.h"

#if defined ARW_WINDOWS
#include "Arrow/Platform/Windows/WindowsLogger.h"
#elif defined ARW_MACOS
#include "Arrow/Platform/MacOS/MacOSLogger.h"
#elif defined ARW_LINUX
#include "Arrow/Platform/Linux/LinuxLogger.h"
#endif

namespace Arrow::Logging
{
	Logger::~Logger()
	{
		for (LogSink* sink : m_Sinks)
			delete sink;
	}

	void Logger::Init()
	{
#if defined ARW_WINDOWS
		s_Logger = new WindowsLogger();
#elif defined ARW_MACOS
        s_Logger = new MacOSLogger();
#elif defined ARW_LINUX
        s_Logger = new LinuxLogger();
#else
		static_assert(false, "There is no logger implementation for this platform!");
#endif
	}

	void Logger::Dispose()
	{
		if (s_Logger)
			delete s_Logger;
	}

	void Logger::FormatTime(char* buffer, tm& time)
	{
		int32_t h = time.tm_hour, m = time.tm_min, s = time.tm_sec;

		// Fill the array with the string template
		buffer[0] = '(';
		buffer[1] = '0';
		buffer[2] = '0';
		buffer[3] = ':';
		buffer[4] = '0';
		buffer[5] = '0';
		buffer[6] = ':';
		buffer[7] = '0';
		buffer[8] = '0';
		buffer[9] = ')';
		buffer[10] = '\0';

		// Hours
		for (int32_t val = (h < 0) ? -h : h, len = 2; len >= 0 && val != 0; --len, val /= 10)
			buffer[len] = '0' + val % 10;

		// Minutes
		for (int32_t val = (m < 0) ? -m : m, len = 2; len >= 0 && val != 0; --len, val /= 10)
			buffer[len + 3] = '0' + val % 10;

		// Seconds
		for (int32_t val = (s < 0) ? -s : s, len = 2; len >= 0 && val != 0; --len, val /= 10)
			buffer[len + 6] = '0' + val % 10;
	}
}
