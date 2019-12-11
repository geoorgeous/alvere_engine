#pragma once

#include <stdexcept>
#include <string>

#include "alvere/debug/logging.hpp"

#ifdef NDEBUG
#define AlvUnreachable(message)
#define AlvAssert(assertion, message) assertion
#else
#define AlvUnreachable(message) throw ::alvere::UnreachableCodeException(__FILE__, __LINE__, message)
#define AlvAssert(assertion, message) (void)((assertion) || (throw ::alvere::AssertionFailedException(#assertion, __FILE__, __LINE__, message),0))
#endif

#define AlvThrow(message, ...) throw ::alvere::Exception(__FILE__, __LINE__, ::alvere::FormatString(message, ##__VA_ARGS__))
#define AlvThrowFatal(message, ...) throw ::alvere::FatalErrorException(__FILE__, __LINE__, ::alvere::FormatString(message, ##__VA_ARGS__))

namespace alvere
{
	class Exception : public std::exception
	{
	public:
		Exception(const char * file, int line, const std::string& message = "");

		const char * what() const noexcept override;

	protected:
		const char * m_File;
		int m_Line;
		std::string m_Message;
		mutable std::string m_FullReport;

	protected:
		virtual std::stringstream& GenerateFullReport(std::stringstream& ss) const;
	};

	class UnreachableCodeException : public Exception
	{
	public:

		UnreachableCodeException(const char * file, int line, const std::string & message = "");

	protected:
		std::stringstream & GenerateFullReport(std::stringstream & ss) const override;
	};

	class AssertionFailedException : public Exception
	{
	public:
		AssertionFailedException(const char * file, int line, const std::string& message = "");
		AssertionFailedException(const char * expression, const char * file, int line, const std::string& message = "");

	protected:
		virtual std::stringstream& GenerateFullReport(std::stringstream& ss) const override;

	private:
		std::string m_AssertionExpression;
	};

	class FatalErrorException : public Exception
	{
	public:
		FatalErrorException(const char * file, int line, const std::string& message = "");

	protected:
		std::stringstream& GenerateFullReport(std::stringstream& ss) const override;
	};
}