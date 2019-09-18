#include "alvere/utils/uuid.hpp"

#include <iomanip>
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

		uint64_t data[2] = { dist(dev), dist(dev) };

		std::memcpy(uuid.m_bytes, data, 16);

		return uuid;
	}

	std::string UUID::toString() const
	{
		std::stringstream ss;
		for (int i = 0; i < 16; ++i)
			ss << std::setfill('0') << std::setw(2) << std::hex << (int)m_bytes[i];
		return ss.str();
	}

	UUID::UUID() { }
}