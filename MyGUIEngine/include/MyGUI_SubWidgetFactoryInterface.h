/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SUB_WIDGET_FACTORY_INTERFACE_H__
#define __MYGUI_SUB_WIDGET_FACTORY_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_SubWidgetInterface.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	class _MyGUIExport SubWidgetFactoryInterface
	{
	public:
		virtual const std::string & getType() = 0;
		virtual SubWidgetInterface * createSubWidget(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent) = 0;
		virtual void * createData(xml::xmlNodePtr _node, xml::xmlNodePtr _root) = 0;
	};

	template <typename T>
	class SubWidgetFactory : public SubWidgetFactoryInterface
	{

	public:
		SubWidgetFactory(const std::string & _type) : mType(_type) { }

		const std::string & getType() { return mType; }

		SubWidgetInterface * createSubWidget(const SubWidgetInfo & _info, CroppedRectangleInterface * _parent)
		{
			return new T(_info, _parent);
		}

		virtual void * createData(xml::xmlNodePtr _node, xml::xmlNodePtr _root)
		{
			return T::createStateData(_node, _root);
		}

	private:
		std::string mType;
	};

} // namespace MyGUI

#endif // __MYGUI_SUB_WIDGET_FACTORY_INTERFACE_H__
