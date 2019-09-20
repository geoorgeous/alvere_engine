#pragma once

#include <typeindex>
#include <unordered_map>

namespace alvere
{
	class Entity;

	class IComponentCollection
	{

	};

	template <typename EntityComponentType>
	class ComponentCollection : public IComponentCollection
	{
	public:

		EntityComponentType & newEntityComponent(Entity * entity)
		{
			m_components.emplace_back(EntityComponentType()).entity = entity;

			return m_components.back();
		}

		const std::vector<EntityComponentType> & components()
		{
			return m_components;
		}

	private:

		std::vector<EntityComponentType> m_components;
	};

	class EntityComponentMap
	{
	public:

		~EntityComponentMap();

		template <typename EntityComponentType>
		ComponentCollection<EntityComponentType> * get()
		{
			std::type_index entityComponentType = typeid(EntityComponentType);

			auto iter = m_entityComponentCollections.find(entityComponentType);

			if (iter == m_entityComponentCollections.end())
			{
				return nullptr;
			}

			return static_cast<ComponentCollection<EntityComponentType> *>(iter->second);
		}

		template <typename EntityComponentType>
		ComponentCollection<EntityComponentType> * getOrCreate()
		{
			std::type_index entityComponentType = typeid(EntityComponentType);

			auto iter = m_entityComponentCollections.find(entityComponentType);

			if (iter == m_entityComponentCollections.end())
			{
				m_entityComponentCollections[entityComponentType] = new ComponentCollection<EntityComponentType>();
			}

			return static_cast<ComponentCollection<EntityComponentType> *>(m_entityComponentCollections[entityComponentType]);
		}

	private:

		std::unordered_map<std::type_index, IComponentCollection *> m_entityComponentCollections;

	};
}