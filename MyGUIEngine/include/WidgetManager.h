#ifndef __WIDGET_MANAGER_H__
#define __WIDGET_MANAGER_H__

#include "Prerequest.h"
#include "Instance.h"
#include "Gui.h"
#include "WidgetFactoryInterface.h"
#include "LoggingOut.h"

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

	class _MyGUIExport WidgetManager
	{
		INSTANCE_HEADER(WidgetManager);

	public:
		void initialise();
		void shutdown();

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String & _name);

		void destroyWidget();
		void destroyWidget(WidgetPtr & _widget);

		inline void registerFactory(WidgetFactoryInterface * _factory)
		{
			mFactoryList.push_back(_factory);
			//LOG_MESSAGE("* Register widget factory '" + _factory->getType() + "'");
		}

		WidgetPtr findWidget(const Ogre::String & _name);

		// преобразует изначальное смещение, в текущее, так как будто скин был создан изначально
		static FloatRect convertOffset(const FloatRect & _offset, Align _align, const IntSize & _parentSkinSize, int _parentWidth, int _parentHeight);

		//	private:
		void clearName(WidgetPtr _widget);

	protected:
		typedef std::list<WidgetFactoryInterface*> ListWidgetFactory;
		ListWidgetFactory mFactoryList;
		MapWidgetPtr mWidgets;

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
