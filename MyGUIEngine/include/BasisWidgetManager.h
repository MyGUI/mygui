#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Prerequest.h"
#include <vector>
//#include <Ogre.h>
#include "BasisWidget.h"
#include "BasisWidgetFactory.h"
#include "Instance.h"

namespace MyGUI
{
	class _MyGUIExport BasisWidgetManager
	{

	public:
		INSTANCE(BasisWidgetManager)

		BasisWidget * createBasisWidget(const tagBasisWidgetInfo &_info, const String & _material, BasisWidget * _parent)
		{
			for (std::list<BasisWidgetFactoryBase*>::iterator factory = m_factoryList.begin(); factory != m_factoryList.end(); factory++) {
				if ((*factory)->getType() == _info.type) return (*factory)->createBasisWidget(_info, _material, _parent);
			}
			assert(0 && "no find factory BasisWidgetFactory");
			return 0;
		}

		inline void registerFactory(BasisWidgetFactoryBase * _factory)
		{
			m_factoryList.push_back(_factory);
		}

	protected:
		BasisWidgetManager() {};
		std::list<BasisWidgetFactoryBase*> m_factoryList;

	}; // BasisWidgetManager

} // namespace MyGUI


#endif