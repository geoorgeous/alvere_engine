#include <string>

#define ALV_LOG_LEVEL_ERRO  0
#define ALV_LOG_LEVEL_WARN  1
#define ALV_LOG_LEVEL_INFO  2
#define ALV_LOG_LEVEL_TRCE  3

#ifndef ALV_LOG_LEVEL
#define ALV_LOG_LEVEL ALV_LOG_LEVEL_TRCE
#endif

#ifndef NDEBUG
#define AlvLogDebug(format, ...) ::alvere::LogDebug(format, __VA_ARGS__)
#else
#define AlvLogDebug(format, ...)
#endif

namespace alvere
{
	std::string FormatString(const char* format, ...);

	void LogContinue(const char* format, ...);

	void LogDebug(const char* format, ...);

	void LogError(const char* format, ...);
	void LogWarning(const char* format, ...);
	void LogInfo(const char* format, ...);
	void LogTrace(const char* format, ...);
}