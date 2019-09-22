#include "alvere/utils/uuid.hpp"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdint.h>

namespace alvere
{
	const UUID UUID::k_invalidUUID = UUID();

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

	std::size_t UUID::createHash() const
	{
		uint64_t * first = (uint64_t *)m_bytes;
		uint64_t * second = (uint64_t *)(m_bytes + 8);

		std::size_t hash = 17 * 31 + std::hash<uint64_t>()(*first);
		return hash *= 31 + std::hash<uint64_t>()(*second);
	}
}