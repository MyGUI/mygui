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
#include "MyGUI_WidgetCreator.h"
#include "MyGUI_UnlinkWidget.h"

namespace MyGUI
{

	// делегат для парсинга
	typedef delegates::CDelegate3<WidgetPtr,  const Ogre::String &, const Ogre::String &> ParseDelegate;

	class _MyGUIExport WidgetManager : public UnlinkWidget
	{
		INSTANCE_HEADER(WidgetManager);

	public:
		typedef std::map<Ogre::String, ParseDelegate> MapDelegate;
		typedef std::set<WidgetFactoryInterface*> SetWidgetFactory;

	public:
		void initialise();
		void shutdown();

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);

		/** Destroy all widgets FIXME or remove - doesn't work*/
		void destroyAllWidget();

		/** Destroy _widget */
		void destroyWidget(WidgetPtr _widget);
		/* Destroy vector of widgets */
		void destroyWidgetsVector(VectorWidgetPtr &_widgets);

		void registerFactory(WidgetFactoryInterface * _factory);
		void unregisterFactory(WidgetFactoryInterface * _factory);

		// метод для поиска виджета
		WidgetPtr findWidgetT(const Ogre::String & _name);
		inline WidgetPtr findWidgetT(const std::string& _name, const std::string& _prefix)
		{
			return findWidgetT(_prefix + _name);
		}
		template <class T> inline T* findWidget(const std::string& _name)
		{
			WidgetPtr widget = findWidgetT(_name);
			if (null == widget) return null;
			MYGUI_DEBUG_ASSERT(null != dynamic_cast<T*>(widget),
				"Error dynamic cast : dest type = '" << T::_getType() 
				<< "' source name = '" << widget->getName() 
				<< "' source type = '" << widget->getWidgetType() << "'");
			return static_cast<T*>(widget);
		}

		template <class T> inline T* findWidget(const std::string& _name, const std::string& _prefix)
		{
			return findWidget<T>(_prefix + _name);
		}

		// преобразует изначальное смещение, в текущее, так как будто скин был создан изначально
		static FloatRect convertOffset(const FloatRect & _offset, Align _align, const IntSize & _parentSkinSize, int _parentWidth, int _parentHeight);
		// преобразует точку на виджете в глобальную позицию
		static IntPoint convertToGlobal(const IntPoint& _point, WidgetPtr _widget);

		// очищает имя в списках
		void _unlinkWidget(WidgetPtr _widget);

		// только удаляет и отписывает виджет
		// метод для тех кто ручками создал виджет
		//void _deleteWidget(WidgetPtr _widget);

		// регестрирует делегат
		ParseDelegate & registerDelegate(const Ogre::String & _key);
		void unregisterDelegate(const Ogre::String & _key);

		// парсит ключ значение
		void parse(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		// все кто хочет отписать у себя виджет при удалении
		void registerUnlinker(UnlinkWidget * _unlink);
		void unregisterUnlinker(UnlinkWidget * _unlink);
		void unlinkFromUnlinkers(WidgetPtr _widget);

	protected:
		SetWidgetFactory mFactoryList;
		MapWidgetPtr mWidgets;
		MapDelegate mDelegates;

		// set of integrated factories
		SetWidgetFactory mIntegratedFactoryList;

		// список менеджеров для отписки при удалении
		VectorUnlinkWidget mVectorUnlinkWidget;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_MANAGER_H__
