#include "alvere/utils/command_console/param.hpp"

namespace alvere::console
{
	IParam::IParam(const std::string & name, const std::string & description, bool isRequired, std::type_index typeIndex, const char * typeString)
		: m_name(name), m_description(description), m_isRequired(isRequired), m_typeIndex(typeIndex)
	{
		m_detailedName = isRequired ? "<()>" : "[()]";
		m_detailedName.insert(2, typeString);
		m_detailedName.insert(1, m_name);
	}
}