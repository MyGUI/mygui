/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
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
#ifndef __MYGUI_WIDGET_MANAGER_H__
#define __MYGUI_WIDGET_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_Widget.h"
#include <set>

namespace MyGUI
{
	class MYGUI_EXPORT WidgetManager :
		public Singleton<WidgetManager>
	{
	public:
		WidgetManager();

		void initialise();
		void shutdown();

		Widget* createWidget(const std::string& _type);

		/** Destroy _widget */
		void destroyWidget(Widget* _widget);
		/** Destroy vector of widgets */
		void destroyWidgets(const VectorWidgetPtr &_widgets);
		/** Destroy Enumerator of widgets */
		void destroyWidgets(EnumeratorWidgetPtr _widgets);

		/** Register unlinker (call unlink if for any destroyed widget)*/
		void registerUnlinker(IUnlinkWidget * _unlink);
		/** Unregister unlinker (call unlink if for any destroyed widget)*/
		void unregisterUnlinker(IUnlinkWidget * _unlink);
		/** Unlink widget */
		void unlinkFromUnlinkers(Widget* _widget);

		/** Check if factory with specified widget type exist */
		bool isFactoryExist(const std::string& _type);

	/*internal:*/
		void _parse(Widget* _widget, const std::string &_key, const std::string &_value);
		void _deleteWidget(Widget* _widget);
		void _deleteDelayWidgets();
		void _deleteWidget2(Widget* _widget);

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void WidgetManager::destroyWidgets(VectorWidgetPtr &_widgets)")
		void destroyWidgetsVector(VectorWidgetPtr &_widgets) { destroyWidgets(_widgets); }
		MYGUI_OBSOLETE("")
		Widget* findWidgetT(const std::string& _name, bool _throw = true);
		MYGUI_OBSOLETE("")
		Widget* findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw = true);
		MYGUI_OBSOLETE("use : void Widget::setProperty(const std::string &_key, const std::string &_value)")
		void parse(Widget* _widget, const std::string &_key, const std::string &_value) { _parse(_widget, _key, _value); }

		template <typename T>
		MYGUI_OBSOLETE("")
		T* findWidget(const std::string& _name, bool _throw = true)
		{
			Widget* widget = findWidgetT(_name, _throw);
			if (nullptr == widget) return nullptr;
			return widget->castType<T>(_throw);
		}

		template <typename T>
		MYGUI_OBSOLETE("")
		T* findWidget(const std::string& _name, const std::string& _prefix, bool _throw = true)
		{
			return findWidget<T>(_prefix + _name, _throw);
		}

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		void notifyEventFrameStart(float _time);

	private:
		bool mIsInitialise;

		// список менеджеров для отписки при удалении
		VectorIUnlinkWidget mVectorIUnlinkWidget;

		// список виджетов для удаления
		VectorWidgetPtr mDestroyWidgets;
	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_MANAGER_H__
