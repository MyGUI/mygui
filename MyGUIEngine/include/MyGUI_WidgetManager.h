/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
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
	typedef delegates::CDelegate3<WidgetPtr,  const Ogre::String &, const Ogre::String &> ParseDelegate;

	class _MyGUIExport WidgetManager : public IUnlinkWidget
	{
		INSTANCE_HEADER(WidgetManager);

	public:
		typedef std::map<Ogre::String, ParseDelegate> MapDelegate;
		typedef std::set<IWidgetFactory*> SetWidgetFactory;

	public:
		void initialise();
		void shutdown();

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

		/** Destroy all widgets FIXME or remove - doesn't work*/
		void destroyAllWidget();

		/** Destroy _widget */
		void destroyWidget(WidgetPtr _widget);
		/** Destroy vector of widgets */
		void destroyWidgets(VectorWidgetPtr &_widgets);
		/** Destroy enumerator of widgets */
		void destroyWidgets(EnumeratorWidgetPtr & _widgets);

		MYGUI_OBSOLETE("use WidgetManager::destroyWidgets(VectorWidgetPtr &_widgets)")
		void destroyWidgetsVector(VectorWidgetPtr &_widgets) { destroyWidgets(_widgets); }

		/** Register widget factory */
		void registerFactory(IWidgetFactory * _factory);
		/** Unregister widget factory */
		void unregisterFactory(IWidgetFactory * _factory);

		// метод для поиска виджета
		/** Find widget by name */
		WidgetPtr findWidgetT(const Ogre::String & _name);
		/** Find widget by name and prefix*/
		WidgetPtr findWidgetT(const std::string& _name, const std::string& _prefix)
		{
			return findWidgetT(_prefix + _name);
		}
		/** Find widget by name and cast it to T type.
			If T and found widget have different types cause error in DEBUG mode.
		*/
		template <typename T> T* findWidget(const std::string& _name)
		{
			WidgetPtr widget = findWidgetT(_name);
			if (null == widget) return null;
			return widget->castType<T>();
		}

		/** Find widget by name and prefix and cast it to T type*/
		template <typename T> T* findWidget(const std::string& _name, const std::string& _prefix)
		{
			return findWidget<T>(_prefix + _name);
		}

		// преобразует точку на виджете в глобальную позицию
		/** Convert position on widget to global position */
		//static IntPoint convertToGlobal(const IntPoint& _point, WidgetPtr _widget);

		// очищает имя в списках
		void _unlinkWidget(WidgetPtr _widget);

		// регестрирует делегат
		/** Register delegate for parsing in layout files or by WidgetManager::parse method 
			@code manager.registerDelegate("Button_Pressed") = newDelegate(this, &ButtonFactory::Button_Pressed); @endcode
		*/
		ParseDelegate & registerDelegate(const Ogre::String & _key);
		/** Unregister delegate for parsing in layout files or by WidgetManager::parse method */
		void unregisterDelegate(const Ogre::String & _key);

		// парсит ключ значение
		/** Parse and apply property to widget
			@param _widget to which property will be applied
			@param _key property
			@param _value for applying
			@code WidgetManager::getInstance()->parse(widget, "Button_Pressed", "true"); @endcode
		*/
		void parse(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		// все кто хочет отписать у себя виджет при удалении
		/** Register unlinker (call unlink if for any destroyed widget)*/
		void registerUnlinker(IUnlinkWidget * _unlink);
		/** Unregister unlinker (call unlink if for any destroyed widget)*/
		void unregisterUnlinker(IUnlinkWidget * _unlink);
		/** Unlink widget */
		void unlinkFromUnlinkers(WidgetPtr _widget);

		/* Convert from relative to pixel coordinates.
			@param
				_coord relative coordinates. (relative to _parent client area coordinates)
			@param
				_parent Widget.
		*/
		IntCoord convertRelativeToInt(const FloatCoord & _coord, WidgetPtr _parent);
		/* Convert from pixel to relative coordinates.
			@param
				_coord relative coordinates. (relative to _parent client area coordinates)
			@param
				_parent Widget.
		*/
		FloatCoord convertIntToRelative(const IntCoord & _coord, WidgetPtr _parent);

	protected:
		SetWidgetFactory mFactoryList;
		MapWidgetPtr mWidgets;
		MapDelegate mDelegates;

		// set of integrated factories
		SetWidgetFactory mIntegratedFactoryList;

		// список менеджеров для отписки при удалении
		VectorIUnlinkWidget mVectorIUnlinkWidget;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_MANAGER_H__
