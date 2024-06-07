#include "Platform/Windows/WindowsLogger.h"

#include <Windows.h>

namespace arwh
{
	void WindowsLogger::PlatformCoreAssert(const std::string& message) const
	{
		MessageBoxA(nullptr, message.c_str(), "Core Assert", MB_OK | MB_ICONERROR);
	}

	void WindowsLogger::PlatformUserAssert(const std::string& message) const
	{
		MessageBoxA(nullptr, message.c_str(), "User Assert", MB_OK | MB_ICONERROR);
	}

	void WindowsLogger::ChangeColor(LogSeverity severity) const
	{
		switch (severity)
		{
		case LogSeverity::Info:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F); // White
			return;
		case LogSeverity::Debug:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02); // Green
			return;
		case LogSeverity::Warning:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x06); // Yellow
			return;
		case LogSeverity::Error:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x04); // Red
			return;
		case LogSeverity::Fatal:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x05); // Purple
			return;
		default:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F); // White
			return;
		}
	}

	void WindowsLogger::ResetColor() const
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F); // White
	}
}