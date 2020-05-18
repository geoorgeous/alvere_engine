#pragma once

#include <string>

#include <alvere/world/component/pooled_component.hpp>

class C_Name : public alvere::PooledComponent<C_Name>
{
public:

	std::string m_Name;
};