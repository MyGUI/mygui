#ifndef _BASICWIDGETFACTORY_H_
#define _BASICWIDGETFACTORY_H_

#include "Prerequest.h"
//#include <Ogre.h>
#include "BasisWidget.h"

namespace MyGUI
{

	class _MyGUIExport BasisWidgetFactoryBase
	{
	public:
		virtual const Ogre::String & getType() = 0;
		virtual BasisWidget * createBasisWidget(const tagBasisWidgetInfo &_info, const String & _material, BasisWidget * _parent) = 0;
	}; // BasisWidgetFactoryBase

	template <class ClassName>
	class BasisWidgetFactory : public BasisWidgetFactoryBase
	{
	public:
		BasisWidgetFactory() { BasisWidgetManager::getInstance().registerFactory(this); }
		inline const Ogre::String & getType() {return ClassName::getType();};
		BasisWidget * createBasisWidget(const tagBasisWidgetInfo &_info, const String & _material, BasisWidget * _parent)
		{
			return new ClassName(_info, _material, _parent);
		}
	}; // class BasisWidgetFactory : public BasisWidgetFactoryBase

} // namespace MyGUI


#endif