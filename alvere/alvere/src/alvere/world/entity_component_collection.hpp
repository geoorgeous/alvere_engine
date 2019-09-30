#pragma once

#include <vector>

namespace alvere
{
	class EntityComponentCollectionBase
	{
	public:

		virtual EntityComponentCollectionBase * createNew() const { return nullptr; }

		virtual unsigned int allocate() { return 0; }

		virtual void removeAt(unsigned int index) { }

		virtual void transferComponent(unsigned int index, EntityComponentCollectionBase * destination, unsigned int destinationIndex) { }

		virtual unsigned int getCount() { return 0; }
	};

	template <typename EntityComponentType>
	class EntityComponentCollection : public EntityComponentCollectionBase
	{
	public:

		inline EntityComponentCollection<EntityComponentType> * createNew() const override
		{
			return new EntityComponentCollection<EntityComponentType>;
		}

		unsigned int allocate() override
		{
			m_components.emplace_back(EntityComponentType());
			return m_components.size() - 1;
		}

		void removeAt(unsigned int index) override
		{
			auto iter = m_components.begin() + index;
			*iter = std::move(m_components.back());
			m_components.pop_back();
		}

		void transferComponent(unsigned int index, EntityComponentCollectionBase * destination, unsigned int destinationIndex) override
		{
			EntityComponentCollection<EntityComponentType> & dest = *static_cast<EntityComponentCollection<EntityComponentType> *>(destination);

			dest.m_components[destinationIndex] = m_components[index];

			auto sourceIter = std::move(m_components.back());
			m_components.pop_back();
		}

		inline unsigned int getCount() override
		{
			return m_components.size();
		}

		inline EntityComponentType & operator[](unsigned int index)
		{
			m_components[index];
		}

	private:

		std::vector<EntityComponentType> m_components;
	};
}