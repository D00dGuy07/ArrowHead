#include "Platform/Linux/LinuxLogger.h"

#include <iostream>

namespace arwh
{
    void LinuxLogger::PlatformCoreAssert(const std::string& message) const {}
    void LinuxLogger::PlatformUserAssert(const std::string& message) const {}


    void LinuxLogger::ChangeColor(LogSeverity severity) const
    {
        switch (severity)
        {
        case LogSeverity::Info:
            std::cout << "\x1b[0m";
            return;
        case LogSeverity::Debug:
            std::cout << "\x1b[32m";
            return;
        case LogSeverity::Warning:
            std::cout << "\x1b[33m";
            return;
        case LogSeverity::Error:
            std::cout << "\x1b[31m";
            return;
        case LogSeverity::Fatal:
            std::cout << "\x1b[35m";
            return;
        default:
            std::cout << "\x1b[0m";
            return;
        }
    }

    void LinuxLogger::ResetColor() const
    {
        std::cout << "\x1b[0m";
    }
}
