/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_ScrollViewFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ScrollViewFactory::ScrollViewFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("ScrollView_VisibleVScroll") = newDelegate(this, &ScrollViewFactory::ScrollView_VScroll);
			manager.registerDelegate("ScrollView_VisibleHScroll") = newDelegate(this, &ScrollViewFactory::ScrollView_HScroll);
			manager.registerDelegate("ScrollView_CanvasAlign") = newDelegate(this, &ScrollViewFactory::ScrollView_CanvasAlign);
			manager.registerDelegate("ScrollView_CanvasSize") = newDelegate(this, &ScrollViewFactory::ScrollView_CanvasSize);

			manager.registerDelegate("ScrollView_VScroll") = newDelegate(this, &ScrollViewFactory::ScrollView_VScroll);
			manager.registerDelegate("ScrollView_HScroll") = newDelegate(this, &ScrollViewFactory::ScrollView_HScroll);
		}

		ScrollViewFactory::~ScrollViewFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("ScrollView_VisibleVScroll");
			manager.unregisterDelegate("ScrollView_VisibleHScroll");
			manager.unregisterDelegate("ScrollView_CanvasAlign");
			manager.unregisterDelegate("ScrollView_CanvasSize");

			manager.unregisterDelegate("ScrollView_VScroll");
			manager.unregisterDelegate("ScrollView_HScroll");
		}

		void ScrollViewFactory::ScrollView_VScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ScrollViewPtr>(_widget)->setVisibleVScroll(utility::parseBool(_value));
		}

		void ScrollViewFactory::ScrollView_HScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ScrollViewPtr>(_widget)->setVisibleHScroll(utility::parseBool(_value));
		}

		void ScrollViewFactory::ScrollView_CanvasAlign(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ScrollViewPtr>(_widget)->setCanvasAlign(Align::parse(_value));
		}

		void ScrollViewFactory::ScrollView_CanvasSize(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ScrollViewPtr>(_widget)->setCanvasSize(IntSize::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
