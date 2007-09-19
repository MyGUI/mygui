
#include "BasisWidgetManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(BasisWidgetManager);

	BasisWidgetManager::BasisWidgetManager() {}

	BasisWidget * BasisWidgetManager::createBasisWidget(const tagBasisWidgetInfo &_info, const String & _material, BasisWidget * _parent)
	{
		for (std::list<BasisWidgetFactoryBase*>::iterator factory = m_factoryList.begin(); factory != m_factoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) return (*factory)->createBasisWidget(_info, _material, _parent);
		}
		OGRE_EXCEPT(0, _info.type + " - no find factory BasisWidgetFactory", "BasisWidgetManager::createBasisWidget");
		return 0;
	}

} // namespace MyGUI