/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_I_SUB_WIDGET_FACTORY_H__
#define __MYGUI_I_SUB_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	class MYGUI_EXPORT ISubWidgetFactory
	{
	public:
		virtual ~ISubWidgetFactory() {};

		virtual const std::string & getTypeName() = 0;
		virtual ISubWidget * createSubWidget(const SubWidgetInfo &_info, ICroppedRectangle * _parent) = 0;
		virtual StateInfo * createData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version) = 0;
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

		virtual StateInfo * createData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
		{
			return T::createStateData(_node, _root, _version);
		}
	};

} // namespace MyGUI

#endif // __MYGUI_I_SUB_WIDGET_FACTORY_H__
