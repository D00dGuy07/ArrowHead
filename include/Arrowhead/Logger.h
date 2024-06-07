#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <type_traits>
#include <sstream>
#include <iomanip>
#include <time.h>

namespace arwh
{
	class LogSink
	{
	public:
		virtual ~LogSink() {}

		virtual std::ostream& GetStream() = 0;
		virtual bool ShouldColorize() const = 0;
	};

	class FileSink : public LogSink
	{
    public:
		FileSink(const char* filePath)
			: m_FileStream(filePath, std::ios::out | std::ios::trunc) {}

		FileSink(const std::string& filePath)
			: FileSink(filePath.c_str()) {}

		virtual ~FileSink() override {}

		virtual std::ostream& GetStream() override { return m_FileStream; }
		virtual bool ShouldColorize() const override { return false; }

	private:
		std::ofstream m_FileStream;
	};

	class StdioSink : public LogSink
	{
	public:
		virtual ~StdioSink() override {}

		virtual std::ostream& GetStream() override { return m_ConsoleStream; }
		virtual bool ShouldColorize() const override { return true; }

	private:
		inline static std::ostream& m_ConsoleStream = std::cout;
	};

	enum class MessageType
	{
		Core,
		User
	};

	enum class LogSeverity
	{
		Info,
		Debug,
		Warning,
		Error,
		Fatal
	};

	class Logger
	{
	public:
        virtual ~Logger();

		template<typename... Args>
		void Log(MessageType type, LogSeverity severity, Args&&... args)
		{
			std::string message = FormatLogMessage(type, severity, std::forward<Args>(args)...);
			for (LogSink* sink : m_Sinks)
			{
				// Print everything with the color indicating severity
				if (sink->ShouldColorize())
				{
					ChangeColor(severity);
					sink->GetStream() << message;
					ResetColor();
				}
				else
					sink->GetStream() << message;
			}
		}

		template<typename... Args>
		void LogTagged(MessageType type, LogSeverity severity, const char* tag, Args&&... args)
		{
			std::string message = FormatLogMessageTagged(type, severity, tag, std::forward<Args>(args)...);
			for (LogSink* sink : m_Sinks)
			{
				// Print everything with the color indicating severity
				if (sink->ShouldColorize())
				{
					ChangeColor(severity);
					sink->GetStream() << message;
					ResetColor();
				}
				else
					sink->GetStream() << message;
			}
		}

		template<typename... Args>
		void LogAssert(MessageType type, Args&&... args)
		{
			std::string message = FormatLogMessage(type, LogSeverity::Fatal, std::forward<Args>(args)...);

			// Log the error
			for (LogSink* sink : m_Sinks)
			{
				// Print everything with the color indicating severity
				if (sink->ShouldColorize())
				{
					ChangeColor(LogSeverity::Fatal);
					sink->GetStream() << message;
					ResetColor();
				}
				else
					sink->GetStream() << message;
			}

			// Do any platform specific tomfoolery
			if (type == MessageType::Core)
				PlatformCoreAssert(message);
			else
				PlatformUserAssert(message);
		}

		template<typename T, typename... Args>
		void AddSink(Args&&... args)
		{
			static_assert(std::is_base_of<LogSink, T>::value, "T must be a subclass of LogSink");
			m_Sinks.push_back(new T(std::forward<Args>(args)...));
		}

		static void Init();
		static inline Logger* Get() { return s_Logger; }
		static void Dispose();

	protected:
		Logger()
			: m_Sinks() {}

		inline static const char* GetTypeString(MessageType type) { return type == MessageType::Core ? "Core" : "User"; }
		inline static const char* GetSeverityString(LogSeverity severity)
		{
			switch (severity)
			{
			case LogSeverity::Info:
				return "Info";
			case LogSeverity::Debug:
				return "Debug";
			case LogSeverity::Warning:
				return "Warn";
			case LogSeverity::Error:
				return "Error";
			case LogSeverity::Fatal:
				return "Fatal";
			default:
				return "Unknown";
			}
		}

		static void FormatTime(char* buffer, tm& time);
		
		template<typename... Args>
		static std::string FormatLogMessage(MessageType type, LogSeverity severity, Args&&... args)
		{
			time_t time = std::time(nullptr);
			tm localTime = tm();
#ifdef _MSC_VER
			localtime_s(&localTime, &time);
#else
            localtime_r(&time, &localTime);
#endif

			std::stringstream message = std::stringstream();

			// Format the time and message type
			char timeString[11];
			FormatTime(timeString, localTime);

			message << timeString;
			message << " [" << GetTypeString(type) << '/' << GetSeverityString(severity) << "]: ";

			// Add the messages
			(message << ... << args);
			message << '\n';

			return message.str();
		}

		template<typename... Args>
		static std::string FormatLogMessageTagged(MessageType type, LogSeverity severity, const char* tag, Args&&... args)
		{
			time_t time = std::time(nullptr);
			tm localTime = tm();
#ifdef _MSC_VER
            localtime_s(&localTime, &time);
#else
            localtime_r(&time, &localTime);
#endif

			std::stringstream message = std::stringstream();

			// Format the time and message type
			char timeString[11];
			FormatTime(timeString, localTime);

			message << timeString;
			message << " [" << GetTypeString(type) << '/' << GetSeverityString(severity) << "]";
			message << " [" << tag << "]: ";

			// Add the messages
			(message << ... << args);
			message << '\n';

			return message.str();
		}

		virtual void PlatformCoreAssert(const std::string& message) const = 0;
		virtual void PlatformUserAssert(const std::string& message) const = 0;

		virtual void ChangeColor(LogSeverity severity) const = 0;
		virtual void ResetColor() const = 0;

		std::vector<LogSink*> m_Sinks;

		inline static Logger* s_Logger;
	};
}

// Logging macros

#define ARWH_LOG_CORE_INFO(...) arwh::Logger::Get()->Log(arwh::MessageType::Core, arwh::LogSeverity::Info, __VA_ARGS__)
#define ARWH_LOG_CORE_DEBUG(...) arwh::Logger::Get()->Log(arwh::MessageType::Core, arwh::LogSeverity::Debug, __VA_ARGS__)
#define ARWH_LOG_CORE_WARN(...) arwh::Logger::Get()->Log(arwh::MessageType::Core, arwh::LogSeverity::Warning, __VA_ARGS__)
#define ARWH_LOG_CORE_ERROR(...) arwh::Logger::Get()->Log(arwh::MessageType::Core, arwh::LogSeverity::Error, __VA_ARGS__)
#define ARWH_LOG_CORE_FATAL(...) arwh::Logger::Get()->Log(arwh::MessageType::Core, arwh::LogSeverity::Fatal, __VA_ARGS__)

#define ARWH_LOG_TAG_CORE_INFO(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::Core, arwh::LogSeverity::Info, tag, __VA_ARGS__)
#define ARWH_LOG_TAG_CORE_DEBUG(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::Core, arwh::LogSeverity::Debug, tag, __VA_ARGS__)
#define ARWH_LOG_TAG_CORE_WARN(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::Core, arwh::LogSeverity::Warning, tag, __VA_ARGS__)
#define ARWH_LOG_TAG_CORE_ERROR(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::Core, arwh::LogSeverity::Error, tag, __VA_ARGS__)
#define ARWH_LOG_TAG_CORE_FATAL(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::Core, arwh::LogSeverity::Fatal, tag, __VA_ARGS__)

#define ARWH_LOG_INFO(...) arwh::Logger::Get()->Log(arwh::MessageType::User, arwh::LogSeverity::Info, __VA_ARGS__)
#define ARWH_LOG_DEBUG(...) arwh::Logger::Get()->Log(arwh::MessageType::User, arwh::LogSeverity::Debug, __VA_ARGS__)
#define ARWH_LOG_WARN(...) arwh::Logger::Get()->Log(arwh::MessageType::User, arwh::LogSeverity::Warning, __VA_ARGS__)
#define ARWH_LOG_ERROR(...) arwh::Logger::Get()->Log(arwh::MessageType::User, arwh::LogSeverity::Error, __VA_ARGS__)
#define ARWH_LOG_FATAL(...) arwh::Logger::Get()->Log(arwh::MessageType::User, arwh::LogSeverity::Fatal, __VA_ARGS__)

#define ARWH_LOG_TAG_INFO(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::User, arwh::LogSeverity::Info, tag, __VA_ARGS__)
#define ARWH_LOG_TAG_DEBUG(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::User, arwh::LogSeverity::Debug, tag, __VA_ARGS__)
#define ARWH_LOG_TAG_WARN(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::User, arwh::LogSeverity::Warning, tag, __VA_ARGS__)
#define ARWH_LOG_TAG_ERROR(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::User, arwh::LogSeverity::Error, tag, __VA_ARGS__)
#define ARWH_LOG_TAG_FATAL(tag, ...) arwh::Logger::Get()->LogTagged(arwh::MessageType::User, arwh::LogSeverity::Fatal, tag, __VA_ARGS__)

// Assert macros

#ifdef _MSC_VER
#define ARWH_DEBUG_BREAK __debugbreak();
#else
#define ARWH_DEBUG_BREAK
#endif

#define ARWH_CORE_ASSERT(condition, ...) { if (!(condition)) { arwh::Logger::Get()->LogAssert(arwh::MessageType::Core, ##__VA_ARGS__); ARWH_DEBUG_BREAK exit(-1); } }
#define ARWH_ASSERT(condition, ...) { if (!(condition)) { arwh::Logger::Get()->LogAssert(arwh::MessageType::User, ##__VA_ARGS__); ARWH_DEBUG_BREAK exit(-1); } }
