#pragma once

#include <memory>

namespace alvere
{
	template <class T>
	using Asset = std::unique_ptr<T>;

	template <class T>
	using AssetRef = T *;

	template <class T>
	using StrongAssetRef = std::shared_ptr<T>;
}