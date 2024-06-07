#pragma once

#include "Arrow/Core/Logger.h"

namespace Arrow::Logging
{
	class WindowsLogger : public Logger
	{
	public:
		virtual void PlatformCoreAssert(const std::string& message) const override;
		virtual void PlatformUserAssert(const std::string& message) const override;

	protected:
		virtual void ChangeColor(LogSeverity severity) const override;
		virtual void ResetColor() const override;
	};
}