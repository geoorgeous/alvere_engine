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
			GenerateFullReport(ss);
			m_FullReport = ss.str();
		}

		return m_FullReport.c_str();
	}

	std::stringstream& Exception::GenerateFullReport(std::stringstream& ss) const
	{
		if (!m_Message.empty())
			ss << m_Message << " ";
		if (ss.rdbuf()->in_avail() == 0)
			ss << "Exception thrown: ";
		ss << "\n\tFile: " << m_File << ", line " << m_Line << ".";
		return ss;
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