#include "Arrow/Platform/MacOS/MacOSLogger.h"

#include <iostream>

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

namespace Arrow::Logging
{
    void MacOSLogger::PlatformCoreAssert(const std::string& message) const
    {
        @autoreleasepool
        {
            NSAlert* alert = [[NSAlert alloc] init];
            [alert setMessageText: @"Core Assert"];
            [alert setInformativeText: [NSString stringWithUTF8String: message.c_str()]];
            [alert setAlertStyle: NSAlertStyleCritical];
            [alert runModal];
        }
    }

    void MacOSLogger::PlatformUserAssert(const std::string& message) const
    {
        @autoreleasepool
        {
            NSAlert* alert = [[NSAlert alloc] init];
            [alert setMessageText: @"User Assert"];
            [alert setInformativeText: [NSString stringWithUTF8String: message.c_str()]];
            [alert setAlertStyle: NSAlertStyleCritical];
            [alert runModal];
        }
    }

    void MacOSLogger::ChangeColor(LogSeverity severity) const
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

    void MacOSLogger::ResetColor() const
    {
        std::cout << "\x1b[0m";
    }
}
