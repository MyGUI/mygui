#pragma once

#include <vector>
#include <Ogre.h>
#include "BasisWidget.h"
#include "BasisWidgetFactory.h"

namespace widget
{
	class BasisWidgetCreator
	{

	public:

		static BasisWidgetCreator & getInstance()
		{
			static BasisWidgetCreator instance;
			return instance;
		}

		BasisWidget * createBasisWidget(const tagBasisWidgetInfo &_info, const String & _material, BasisWidget * _parent)
		{
			for (FactoryIterator factory = m_factoryList.begin(); factory != m_factoryList.end(); factory++) {
				if ((*factory)->getType() == _info.type) return (*factory)->createBasisWidget(_info, _material, _parent);
			}
			assert(0 && "no find factory BasisWidgetFactory");
			return 0;
		}

		void registerFactory(BasisWidgetFactory * _factory)
		{
			m_factoryList.push_back(_factory);
		}

	protected:
		BasisWidgetCreator() {};

		typedef std::list<BasisWidgetFactory*> FactoryList;
		typedef FactoryList::iterator FactoryIterator;

		FactoryList m_factoryList;

	}; // BasisWidgetCreator

} // namespace widget