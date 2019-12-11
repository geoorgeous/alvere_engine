#include <sstream>

#include "alvere/utils/exceptions.hpp"

namespace alvere
{
	Exception::Exception(const char * file, int line, const std::string& message)
		: m_File(file), m_Line(line), m_Message(message) { }

	const char * Exception::what() const noexcept
	{
		if (m_FullReport.empty())
		{
			std::stringstream ss;
			m_FullReport = GenerateFullReport(ss).str();
		}

		return m_FullReport.c_str();
	}

	std::stringstream& Exception::GenerateFullReport(std::stringstream& ss) const
	{
		if (ss.rdbuf()->in_avail() == 0)
			ss << "Exception thrown.";

		ss << " Location: " << m_File << "::" << m_Line << ".";

		if (!m_Message.empty())
			ss << " Message: " << m_Message;

		return ss;
	}

	UnreachableCodeException::UnreachableCodeException(const char * file, int line, const std::string & message)
		: Exception(file, line, message)
	{ }

	std::stringstream & UnreachableCodeException::GenerateFullReport(std::stringstream & ss) const
	{
		ss << "Unreachable code detected.";
		return Exception::GenerateFullReport(ss);
	}

	AssertionFailedException::AssertionFailedException(const char* file, int line, const std::string& message)
		: Exception(file, line, message)
	{ }

	AssertionFailedException::AssertionFailedException(const char* expression, const char* file, int line, const std::string& message)
		: m_AssertionExpression(expression), Exception(file, line, message)
	{ }

	std::stringstream& AssertionFailedException::GenerateFullReport(std::stringstream& ss) const
	{
		if (!m_AssertionExpression.empty())
			ss << "Failed to assert \"" << m_AssertionExpression << "\". ";
		return Exception::GenerateFullReport(ss);
	}

	FatalErrorException::FatalErrorException(const char * file, int line, const std::string& message)
		: Exception(file, line, message)
	{ }

	std::stringstream& FatalErrorException::GenerateFullReport(std::stringstream& ss) const
	{
		ss << "FATAL ERROR! ";
		return Exception::GenerateFullReport(ss);
	}
}