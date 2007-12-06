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
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetFactoryInterface.h"

#include "MyGUI_WidgetFactory.h"
#include "MyGUI_ButtonFactory.h"
#include "MyGUI_EditFactory.h"
#include "MyGUI_ListFactory.h"
#include "MyGUI_StaticImageFactory.h"
#include "MyGUI_StaticTextFactory.h"
#include "MyGUI_VScrollFactory.h"
#include "MyGUI_HScrollFactory.h"
#include "MyGUI_WindowFactory.h"

namespace MyGUI
{

	// делегат для парсинга
	typedef delegates::CDelegate3<WidgetPtr,  const Ogre::String &, const Ogre::String &> ParseDelegate;

	class _MyGUIExport WidgetManager
	{
		INSTANCE_HEADER(WidgetManager);

	public:
		typedef std::map<Ogre::String, ParseDelegate> MapDelegate;
		typedef std::set<WidgetFactoryInterface*> SetWidgetFactory;

	public:
		void initialise();
		void shutdown();

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, CroppedRectanglePtr _parent, const Ogre::String & _name);

		void destroyAllWidget();
		void destroyWidget(WidgetPtr & _widget);

		void registerFactory(WidgetFactoryInterface * _factory);
		void unregisterFactory(WidgetFactoryInterface * _factory);

		// метод для поиска виджета
		WidgetPtr findWidget(const Ogre::String & _name);
		// шаблонный метод для поиска виджета
		template <class T>
		inline T* findWidgetT(const Ogre::String & _name)
		{
			Widget * widget = findWidget(_name);
			MYGUI_TYPE(T*, widget);
			return static_cast<T*>(widget);
		}

		// преобразует изначальное смещение, в текущее, так как будто скин был создан изначально
		static FloatRect convertOffset(const FloatRect & _offset, Align _align, const IntSize & _parentSkinSize, int _parentWidth, int _parentHeight);

		// очищает имя в списках
		void clearName(WidgetPtr _widget);

		// регестрирует делегат
		ParseDelegate & registerDelegate(const Ogre::String & _key);
		void unregisterDelegate(const Ogre::String & _key);

		// парсит ключ значение
		void parse(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);


	protected:
		SetWidgetFactory mFactoryList;
		MapWidgetPtr mWidgets;
		MapDelegate mDelegates;


		// фабрики виджетов
		factory::WidgetFactory * mWidgetFactory;
		factory::ButtonFactory * mButtonFactory;
		factory::EditFactory * mEditFactory;
		factory::ListFactory * mListFactory;
		factory::StaticTextFactory * mStaticTextFactory;
		factory::StaticImageFactory * mStaticImageFactory;
		factory::VScrollFactory * mVScrollFactory;
		factory::HScrollFactory * mHScrollFactory;
		factory::WindowFactory * mWindowFactory;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_MANAGER_H__