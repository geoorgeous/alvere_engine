#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "alvere/utils/logging.hpp"

namespace alvere::assets
{
	class IAsset
	{
	public:

		virtual ~IAsset() = 0
		{ };

		virtual void freeAsset() = 0;
	};

	template <typename AssetType>
	class Asset : public IAsset
	{
	public:

		static Asset<AssetType> s_invalidAsset;

		Asset(AssetType * asset, const std::string & filepath = "BUILT_IN_ASSET")
			: m_asset(asset), m_filepath(filepath)
		{ }

		AssetType * getAssetPtr()
		{
			return m_asset;
		}

		const AssetType * getAssetPtr() const
		{
			return m_asset;
		}

		const std::string & getFilepath() const
		{
			return m_filepath;
		}

		bool isValid() const
		{
			return m_asset != nullptr;
		}

		AssetType & operator->()
		{
			return m_asset.get();
		}

		const AssetType & operator->() const
		{
			return m_asset.get();
		}

		void freeAsset()
		{
			delete m_asset;
		}

	private:

		AssetType * m_asset;

		std::string m_filepath;
	};

	template <typename AssetType>
	Asset<AssetType> Asset<AssetType>::s_invalidAsset = Asset<AssetType>(nullptr, "INVALID_ASSET");

	class AssetManager
	{
	public:

		inline static AssetManager & getInstance()
		{
			return s_instance;
		}

		~AssetManager();

		template <typename AssetType>
		Asset<AssetType> add(AssetType * asset, const std::string & id)
		{
			if (m_assets.find(id) == m_assets.end())
				m_assets[id] = new Asset<AssetType>(asset);

			LogWarning("Failed to add managed asset: asset with id '%s' already exists.", id.c_str());

			return Asset<AssetType>::s_invalidAsset;
		}

		template <typename AssetType>
		Asset<AssetType> get(const std::string & id)
		{
			auto foundAssetIter = m_assets.find(id);

			if (foundAssetIter != m_assets.end())
			{
				Asset<AssetType> * castedAsset = dynamic_cast<Asset<AssetType> *>(foundAssetIter->second);

				if (castedAsset != nullptr)
					return *castedAsset;

				LogWarning("Failed to get managed asset: Type mismatch between requested type and managed asset '%s'.", id.c_str());

				return Asset<AssetType>::s_invalidAsset;
			}

			Asset<AssetType> * loadedAsset = new Asset<AssetType>(AssetType::loadFromFile(id), id);

			if (loadedAsset->isValid())
				return *dynamic_cast<Asset<AssetType> *>(m_assets.emplace(id, loadedAsset).first->second);

			LogWarning("Failed to get managed asset: No asset exists with id '%s', and failed to load from file.", id.c_str());

			delete loadedAsset;

			return Asset<AssetType>::s_invalidAsset;
		}

	private:

		static AssetManager s_instance;

		std::unordered_map<std::string, IAsset *> m_assets;

		AssetManager();
	};
}