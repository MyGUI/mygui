#ifndef __WIDGET_MANAGER_H__
#define __WIDGET_MANAGER_H__

#include "Prerequest.h"
#include "Instance.h"
#include "Gui.h"
#include "WidgetFactoryInterface.h"
#include "LoggingOut.h"
#include "delegate.h"

#include "WidgetFactory.h"
#include "ButtonFactory.h"
#include "EditFactory.h"
#include "ListFactory.h"
#include "StaticImageFactory.h"
#include "VScrollFactory.h"
#include "HScrollFactory.h"
#include "WindowFactory.h"

namespace MyGUI
{

	// делегат для парсинга
	typedef CDelegate3<WidgetPtr,  const Ogre::String &, const Ogre::String &> ParseDelegate;

	class _MyGUIExport WidgetManager
	{
		INSTANCE_HEADER(WidgetManager);

	public:
		typedef std::map<Ogre::String, ParseDelegate> MapDelegate;
		typedef std::set<WidgetFactoryInterface*> SetWidgetFactory;

	public:
		void initialise();
		void shutdown();

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String & _name);

		void destroyWidget();
		void destroyWidget(WidgetPtr & _widget);

		void registerFactory(WidgetFactoryInterface * _factory);
		void unregisterFactory(WidgetFactoryInterface * _factory);

		WidgetPtr findWidget(const Ogre::String & _name);

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
		factory::StaticImageFactory * mStaticImageFactory;
		factory::VScrollFactory * mVScrollFactory;
		factory::HScrollFactory * mHScrollFactory;
		factory::WindowFactory * mWindowFactory;

	};

} // namespace MyGUI

#endif // __WIDGET_MANAGER_H__
