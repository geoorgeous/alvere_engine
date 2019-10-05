#include "alvere/utils/logging.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdarg.h>
#include <stddef.h>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "alvere/utils/console.hpp"

#define ALV_LOG_TIMESTAMP_FMT "[%X]"
#define ALV_LOG_PREFIXSTRING_DBUG "[## Debug ##] "
#define ALV_LOG_PREFIXSTRING_ERRO "[ERROR] "
#define ALV_LOG_PREFIXSTRING_WARN "[Warning] "
#define ALV_LOG_PREFIXSTRING_INFO "[Info] "
#define ALV_LOG_PREFIXSTRING_TRCE "[Trace] "

namespace alvere
{
	enum class LogType : short
	{
#ifndef NDEBUG
		Debug, // Compiles out of release build, should not be long term and should frequently be cleaned up.
#endif
		Error, // Is an issue, please investigate and resolve.
		Warning, // Could be causing an issue, might be worth investigating.
		Info, // Provide more general and helpful info on the state of the application.
		Trace, // Provides trace info on specific parts of the application.
	};

	static std::unordered_map<LogType, std::ostringstream> s_LogOutStreams;
	static LogType s_LastUsedLogType = LogType::Trace;
	static __time64_t s_ClockTime;
	static bool s_isInit;
#if defined(ALV_LOG_OUTFILE)
	static std::ofstream s_OutFile;
#endif
	static std::unique_ptr<console::Command> s_logCommand;

	void init()
	{
		if (s_isInit)
			return;
		s_isInit = true;

#if defined(ALV_LOG_OUTFILE)
		s_OutFile = std::ofstream(ALV_LOG_OUTFILE);
		std::cout.rdbuf(s_OutFile.rdbuf());
#endif

		s_logCommand = std::make_unique<console::Command>("log", "Log text to the info log buffer.", std::vector<console::Command::IParam *> {
			&console::Command::Param<std::string>("text", "The text that will be submitted to the info log buffer.", true) },
			[&](std::vector<const console::Command::IArg *> args) -> std::string
		{
			std::string output = args[0]->getValue<std::string>(); + "\n";
			LogInfo(output.c_str());
			return "";
		});
	}

	std::string FormatString(const char* format, va_list args)
	{
		size_t len = std::vsnprintf(NULL, 0, format, args);
		std::vector<char> vec(len + 1);
		std::vsnprintf(&vec[0], len + 1, format, args);
		return std::string(&vec[0]);
	}

	std::_Timeobj<char, const tm *> GetFormattedTimeStamp()
	{
		s_ClockTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		return std::put_time(std::localtime(&s_ClockTime), ALV_LOG_TIMESTAMP_FMT);
	}

	void NewLog(LogType type, std::string message)
	{
		if (!s_isInit) init();

		s_LastUsedLogType = type;

		std::stringstream ss;
		ss << GetFormattedTimeStamp();

		switch (type)
		{
		case LogType::Trace:	ss << ALV_LOG_PREFIXSTRING_TRCE; break;
		case LogType::Info:		ss << ALV_LOG_PREFIXSTRING_INFO; break;
		case LogType::Warning:	ss << ALV_LOG_PREFIXSTRING_WARN; break;
		case LogType::Error:	ss << ALV_LOG_PREFIXSTRING_ERRO; break;
#ifndef NDEBUG
		case LogType::Debug:	ss << ALV_LOG_PREFIXSTRING_DBUG; break;
#endif
		}

		ss << message;

		s_LogOutStreams[type] << ss.str();
		std::cout << ss.str();

#if defined(ALV_LOG_OUTFILE)
		s_OutFile.flush();
#endif
	}

#define VA_START(fmt) va_list args; va_start(args, fmt)
#define VA_END(fmt) va_end(args, fmt)
#define VA_FORMATTED_STR(fmt) VA_START(fmt); std::string str(FormatString(fmt, args)); VA_END(fmt)

	std::string FormatString(const char* format, ...)
	{
		VA_FORMATTED_STR(format);
		return str;
	}

	void LogContinue(const char* format, ...)
	{
		VA_FORMATTED_STR(format);
		s_LogOutStreams[s_LastUsedLogType] << str;
		std::cout << str;
	}

#define LOG_FUNC_EMPTY(log_type) void Log##log_type(const char* format, ...) { }
#define LOG_FUNC(log_type) void Log##log_type(const char* format, ...)\
	{\
		VA_FORMATTED_STR(format);\
		NewLog(LogType::##log_type, str);\
	}

#ifndef NDEBUG
	LOG_FUNC(Debug)
#else
		LOG_FUNC_EMPTY(Debug)
#endif

#if (ALV_LOG_LEVEL < ALV_LOG_LEVEL_ERRO)
	LOG_FUNC_EMPTY(Error)
#else
	LOG_FUNC(Error)
#endif

#if (ALV_LOG_LEVEL < ALV_LOG_LEVEL_WARN)
	LOG_FUNC_EMPTY(Warning)
#else
	LOG_FUNC(Warning)
#endif

#if (ALV_LOG_LEVEL < ALV_LOG_LEVEL_INFO)
	LOG_FUNC_EMPTY(Info)
#else
	LOG_FUNC(Info)
#endif

#if (ALV_LOG_LEVEL < ALV_LOG_LEVEL_TRCE)
	LOG_FUNC_EMPTY(Trace)
#else
	LOG_FUNC(Trace)
#endif
}