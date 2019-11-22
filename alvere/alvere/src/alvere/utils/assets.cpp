#include "alvere/utils/assets.hpp"

namespace alvere::assets
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