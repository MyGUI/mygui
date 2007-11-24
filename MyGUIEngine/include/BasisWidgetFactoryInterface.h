#ifndef __BASIS_WIDGET_FACTORY_INTERFACE_H__
#define __BASIS_WIDGET_FACTORY_INTERFACE_H__

#include "Prerequest.h"
#include "BasisWidget.h"

namespace MyGUI
{

	class _MyGUIExport BasisWidgetFactoryInterface
	{
	public:
		virtual const Ogre::String & getType() = 0;
		virtual BasisWidget * createBasisWidget(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidget * _parent) = 0;
	};

	template <class ClassName>
	class BasisWidgetFactory : public BasisWidgetFactoryInterface
	{
	public:
		const Ogre::String & getType() {return ClassName::getType();};
		BasisWidget * createBasisWidget(const BasisWidgetInfo& _info, const Ogre::String& _material, BasisWidget* _parent)
		{
			return new ClassName(_info, _material, _parent);
		}
	}; // class BasisWidgetFactory : public BasisWidgetFactoryInterface

} // namespace MyGUI

#endif // __BASIS_WIDGET_FACTORY_INTERFACE_H__