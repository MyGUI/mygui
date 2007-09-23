#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Prerequest.h"
#include "Instance.h"
#include <vector>
#include "BasisWidget.h"
#include "BasisWidgetFactory.h"

namespace MyGUI
{
	class _MyGUIExport BasisWidgetManager// : public Instance<BasisWidgetManager>
	{
		INSTANCE_HEADER(BasisWidgetManager);

	public:

		BasisWidget * createBasisWidget(const tagBasisWidgetInfo &_info, const String & _material, BasisWidget * _parent);
		/*{
			for (std::list<BasisWidgetFactoryBase*>::iterator factory = m_factoryList.begin(); factory != m_factoryList.end(); factory++) {
				if ((*factory)->getType() == _info.type) return (*factory)->createBasisWidget(_info, _material, _parent);
			}
			OGRE_EXCEPT(0, _info.type + " - no find factory BasisWidgetFactory", "BasisWidgetManager::createBasisWidget");
			return 0;
		}*/

		inline void registerFactory(BasisWidgetFactoryBase * _factory)
		{
			m_factoryList.push_back(_factory);
		}

	protected:
		BasisWidgetManager();// {};
		std::list<BasisWidgetFactoryBase*> m_factoryList;

	}; // BasisWidgetManager

} // namespace MyGUI


#endif