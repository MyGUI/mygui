/*!
	@file
	@author		Albert Semenov
	@date		01/2008
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
#ifndef __MYGUI_TAB_ITEM_FACTORY_H__
#define __MYGUI_TAB_ITEM_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_TabItem.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT TabItemFactory : public BaseWidgetFactory<TabItem>
		{
		public:
			TabItemFactory();
			~TabItemFactory();

		private:
			// методы для парсинга
			void TabItem_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void TabItem_Select(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

		// OBSOLETE
		class MYGUI_EXPORT SheetFactory : public IWidgetFactory
		{
		public:
			SheetFactory();
			const std::string & getTypeName()
			{
				static std::string type = "Sheet";
				return type;
			}
			WidgetPtr createWidget(WidgetStyle _style, const std::string& _skin, const IntCoord& _coord, Align _align, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
			{
				MYGUI_LOG(Warning, "Type 'Sheet' is obsolette, use 'TabItem'");
				return new TabItem(_style, _coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _croppedParent, _creator, _name);
			}
		};


	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_TAB_ITEM_FACTORY_H__
