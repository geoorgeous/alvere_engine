#include "alvere/utils/assets.hpp"

namespace alvere
{
	AssetManager AssetManager::s_instance;

	AssetManager::~AssetManager()
	{
		for (auto & it : m_assets)
		{
			it.second->freeAsset();
			delete it.second;
		}
	}

	AssetManager::AssetManager() { }
}