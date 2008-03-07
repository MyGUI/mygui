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
#include "MyGUI_UnlinkWidget.h"

namespace MyGUI
{

	// ������� ��� ��������
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

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String & _name);

		void destroyAllWidget();
		void destroyWidget(WidgetPtr _widget);

		void registerFactory(WidgetFactoryInterface * _factory);
		void unregisterFactory(WidgetFactoryInterface * _factory);

		// ����� ��� ������ �������
		WidgetPtr findWidgetT(const Ogre::String & _name);
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

		// ����������� ����������� ��������, � �������, ��� ��� ����� ���� ��� ������ ����������
		static FloatRect convertOffset(const FloatRect & _offset, Align _align, const IntSize & _parentSkinSize, int _parentWidth, int _parentHeight);
		// ����������� ����� �� ������� � ���������� �������
		static IntPoint convertToGlobal(const IntPoint& _point, WidgetPtr _widget);

		// ������� ��� � �������
		void _unlinkWidget(WidgetPtr _widget);

		// ������������ �������
		ParseDelegate & registerDelegate(const Ogre::String & _key);
		void unregisterDelegate(const Ogre::String & _key);

		// ������ ���� ��������
		void parse(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		// ��� ��� ����� �������� � ���� ������ ��� ��������
		void registerUnlinker(UnlinkWidget * _unlink);
		void unregisterUnlinker(UnlinkWidget * _unlink);
		void unlinkFromUnlinkers(WidgetPtr _widget);

	protected:
		SetWidgetFactory mFactoryList;
		MapWidgetPtr mWidgets;
		MapDelegate mDelegates;

		// set of integrated factories
		SetWidgetFactory mIntegratedFactoryList;

		// ������ ���������� ��� ������� ��� ��������
		VectorUnlinkWidget mVectorUnlinkWidget;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_MANAGER_H__
