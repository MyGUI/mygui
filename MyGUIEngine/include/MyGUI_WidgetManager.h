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
#ifndef __MYGUI_WIDGET_MANAGER_H__
#define __MYGUI_WIDGET_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_IWidgetCreator.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	// делегат для парсинга
	typedef delegates::CDelegate3<WidgetPtr,  const std::string &, const std::string &> ParseDelegate;

	class MYGUI_EXPORT WidgetManager : public IUnlinkWidget
	{
		MYGUI_INSTANCE_HEADER(WidgetManager);

	public:
		typedef std::map<std::string, ParseDelegate> MapDelegate;
		typedef std::set<IWidgetFactory*> SetWidgetFactory;

	public:
		void initialise();
		void shutdown();

		WidgetPtr createWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, WidgetPtr _parent, ICroppedRectangle * _cropeedParent, IWidgetCreator * _creator, const std::string & _name);

		/** Destroy all widgets FIXME or remove - doesn't work*/
		void destroyAllWidget();

		/** Destroy _widget */
		void destroyWidget(WidgetPtr _widget);
		/** Destroy vector of widgets */
		void destroyWidgets(const VectorWidgetPtr &_widgets);
		/** Destroy Enumerator of widgets */
		void destroyWidgets(EnumeratorWidgetPtr _widgets);

		MYGUI_OBSOLETE("use : void WidgetManager::destroyWidgets(VectorWidgetPtr &_widgets)")
		void destroyWidgetsVector(VectorWidgetPtr &_widgets) { destroyWidgets(_widgets); }

		/** Register widget factory */
		void registerFactory(IWidgetFactory * _factory);
		/** Unregister widget factory */
		void unregisterFactory(IWidgetFactory * _factory);

		// метод для поиска виджета
		/** Find widget by name
			If widget is not found the exception will be thrown, or if the second parameter is false the nullptr pointer will be returned
		*/
		WidgetPtr findWidgetT(const std::string & _name, bool _throw = true);
		/** Find widget by name and prefix*/
		WidgetPtr findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw = true)
		{
			return findWidgetT(_prefix + _name, _throw);
		}
		/** Find widget by name and cast it to T type.
			If T and found widget have different types cause error in DEBUG mode.
		*/
		template <typename T>
		T* findWidget(const std::string& _name, bool _throw = true)
		{
			WidgetPtr widget = findWidgetT(_name, _throw);
			if (nullptr == widget) return nullptr;
			return widget->castType<T>(_throw);
		}

		/** Find widget by name and prefix and cast it to T type*/
		template <typename T>
		T* findWidget(const std::string& _name, const std::string& _prefix, bool _throw = true)
		{
			return findWidget<T>(_prefix + _name, _throw);
		}

		// очищает имя в списках
		void _unlinkWidget(WidgetPtr _widget);

		// регестрирует делегат
		/** Register delegate for parsing in layout files or by WidgetManager::parse method
			@code manager.registerDelegate("Button_Pressed") = newDelegate(this, &ButtonFactory::Button_Pressed); @endcode
		*/
		ParseDelegate & registerDelegate(const std::string & _key);
		/** Unregister delegate for parsing in layout files or by WidgetManager::parse method */
		void unregisterDelegate(const std::string & _key);

		// парсит ключ значение
		/** Parse and apply property to widget
			@param _widget to which property will be applied
			@param _key property
			@param _value for applying
			@code WidgetManager::getInstance()->parse(widget, "Button_Pressed", "true"); @endcode
		*/
		void parse(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		// все кто хочет отписать у себя виджет при удалении
		/** Register unlinker (call unlink if for any destroyed widget)*/
		void registerUnlinker(IUnlinkWidget * _unlink);
		/** Unregister unlinker (call unlink if for any destroyed widget)*/
		void unregisterUnlinker(IUnlinkWidget * _unlink);
		/** Unlink widget */
		void unlinkFromUnlinkers(WidgetPtr _widget);

		/* Convert from relative to pixel coordinates.
			@param _coord relative coordinates. (relative to _parent client area coordinates)
			@param _parent Widget.
		*/
		IntCoord convertRelativeToInt(const FloatCoord & _coord, WidgetPtr _parent);
		/* Convert from pixel to relative coordinates.
			@param _coord relative coordinates. (relative to _parent client area coordinates)
			@param _parent Widget.
		*/
		FloatCoord convertIntToRelative(const IntCoord & _coord, WidgetPtr _parent);

		// добавляет виджет в список для анлинка
		void addWidgetToUnlink(WidgetPtr _widget)
		{
			if (_widget) mUnlinkWidgets.push_back(_widget);
		}

		// проверяет, и если надо обнуляет виджет из списка анликнутых
		void removeWidgetFromUnlink(WidgetPtr & _widget)
		{
			VectorWidgetPtr::iterator iter = std::find(mUnlinkWidgets.begin(), mUnlinkWidgets.end(), _widget);
			if (iter != mUnlinkWidgets.end()) {
				(*iter) = mUnlinkWidgets.back();
				mUnlinkWidgets.pop_back();
			}
			else {
				_widget = nullptr;
			}
		}

	protected:
		SetWidgetFactory mFactoryList;
		MapWidgetPtr mWidgets;
		MapDelegate mDelegates;

		// set of integrated factories
		SetWidgetFactory mIntegratedFactoryList;

		// список менеджеров для отписки при удалении
		VectorIUnlinkWidget mVectorIUnlinkWidget;

		// список виджетов для отписки
		VectorWidgetPtr mUnlinkWidgets;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_MANAGER_H__
