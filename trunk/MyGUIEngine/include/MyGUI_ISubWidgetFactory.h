/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_I_SUB_WIDGET_FACTORY_H__
#define __MYGUI_I_SUB_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	class _MyGUIExport ISubWidgetFactory
	{
	public:
		virtual const std::string & getTypeName() = 0;
		virtual ISubWidget * createSubWidget(const SubWidgetInfo &_info, ICroppedRectangle * _parent) = 0;
		virtual StateInfo * createData(xml::xmlNodePtr _node, xml::xmlNodePtr _root) = 0;
	};

	template <typename T>
	class SubWidgetFactory : public ISubWidgetFactory
	{

	public:

		const std::string & getTypeName()
		{
			return T::getClassTypeName();
		}

		ISubWidget * createSubWidget(const SubWidgetInfo & _info, ICroppedRectangle * _parent)
		{
			return new T(_info, _parent);
		}

		virtual StateInfo * createData(xml::xmlNodePtr _node, xml::xmlNodePtr _root)
		{
			return T::createStateData(_node, _root);
		}
	};

} // namespace MyGUI

#endif // __MYGUI_I_SUB_WIDGET_FACTORY_H__
