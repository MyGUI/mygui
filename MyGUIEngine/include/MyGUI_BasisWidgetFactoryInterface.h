/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_BASIS_WIDGET_FACTORY_INTERFACE_H__
#define __MYGUI_BASIS_WIDGET_FACTORY_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_BasisWidget.h"

namespace MyGUI
{

	class _MyGUIExport BasisWidgetFactoryInterface
	{
	public:
		virtual void getNextId(size_t & _id) = 0;
		virtual const Ogre::String & getType() = 0;
		virtual BasisWidget * createBasisWidget(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidget * _parent, size_t & _id) = 0;
	};

	template <class ClassName>
	class BasisWidgetFactory : public BasisWidgetFactoryInterface
	{
	public:
		void getNextId(size_t & _id) {if (ClassName::_isSharedOverlay())_id++;}
		const Ogre::String & getType() {return ClassName::_getType();};
		BasisWidget * createBasisWidget(const BasisWidgetInfo& _info, const Ogre::String& _material, BasisWidget* _parent, size_t & _id)
		{
			ClassName * obj = new ClassName(_info, _material, _parent, _id);
			getNextId(_id);
			return  obj;
		}
	}; // class BasisWidgetFactory : public BasisWidgetFactoryInterface

} // namespace MyGUI

#endif // __MYGUI_BASIS_WIDGET_FACTORY_INTERFACE_H__