#include "alvere/utils/uuid.hpp"

#include <ios>
#include <iostream>
#include <random>
#include <sstream>
#include <stdint.h>

namespace alvere
{
	UUID UUID::create()
	{
		UUID uuid;

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<uint64_t> dist;

		for (int i = 0; i < 2; ++i)
		{
			uint64_t n = dist(rng);
			unsigned char * nc = (unsigned char *)&n;

			for (int j = 0; j < 8; ++j)
				uuid.m_bytes[i * 8 + j] = *(nc + j);
		}

		return uuid;
	}

	std::string UUID::toString() const
	{
		std::stringstream ss;
		for (int i = 0; i < 16; ++i)
			ss << std::hex << (int)m_bytes[i];
		return ss.str();
	}

	UUID::UUID()
		: m_bytes()
	{
		for (int i = 0; i < 16; ++i)
			m_bytes[i] = 0;
	}
}