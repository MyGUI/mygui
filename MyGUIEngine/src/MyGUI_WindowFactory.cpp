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
#include "MyGUI_Precompiled.h"
#include "MyGUI_WindowFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		WindowFactory::WindowFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("Window_AutoAlpha") = newDelegate(this, &WindowFactory::Window_AutoAlpha);
			manager.registerDelegate("Window_Snap") = newDelegate(this, &WindowFactory::Window_Snap);
			manager.registerDelegate("Window_MinMax") = newDelegate(this, &WindowFactory::Window_MinMax);
			manager.registerDelegate("Window_MinSize") = newDelegate(this, &WindowFactory::Window_MinSize);
			manager.registerDelegate("Window_MaxSize") = newDelegate(this, &WindowFactory::Window_MaxSize);
		}

		WindowFactory::~WindowFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("Window_AutoAlpha");
			manager.unregisterDelegate("Window_Snap");
			manager.unregisterDelegate("Window_MinMax");
			manager.unregisterDelegate("Window_MinSize");
			manager.unregisterDelegate("Window_MaxSize");
		}

		void WindowFactory::Window_AutoAlpha(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<WindowPtr>(_widget)->setAutoAlpha(utility::parseBool(_value));
		}

		void WindowFactory::Window_Snap(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<WindowPtr>(_widget)->setSnap(utility::parseBool(_value));
		}

		void WindowFactory::Window_MinMax(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			IntRect rect = IntRect::parse(_value);
			static_cast<WindowPtr>(_widget)->setMinSize(rect.left, rect.top);
			static_cast<WindowPtr>(_widget)->setMaxSize(rect.right, rect.bottom);
		}

		void WindowFactory::Window_MinSize(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<WindowPtr>(_widget)->setMinSize(IntSize::parse(_value));
		}

		void WindowFactory::Window_MaxSize(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<WindowPtr>(_widget)->setMaxSize(IntSize::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
