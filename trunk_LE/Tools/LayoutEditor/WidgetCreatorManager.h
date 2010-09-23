/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __WIDGET_CREATOR_MANAGER_H__
#define __WIDGET_CREATOR_MANAGER_H__

/*#include <MyGUI.h>
#include "WidgetContainer.h"
#include "PositionSelectorControl.h"

namespace tools
{
	typedef MyGUI::delegates::CMultiDelegate1<bool> Event_ChangeCreatorMode;

	class WidgetCreatorManager :
		public MyGUI::Singleton<WidgetCreatorManager>
	{
	public:
		WidgetCreatorManager();
		virtual ~WidgetCreatorManager();

		void initialise();
		void shutdown();

		void notifyMouseMouseMove(const MyGUI::IntPoint& _point);
		void notifyMouseMouseDrag(const MyGUI::IntPoint& _point);
		void notifyMouseButtonPressed(const MyGUI::IntPoint& _point);
		void notifyMouseButtonReleased(const MyGUI::IntPoint& _point);

		void setCreatorInfo(const std::string& _widgetType, const std::string& _widgetSkin);
		void resetCreatorInfo();

		bool getCreateMode();
		const std::string& getWidgetType();
		const std::string& getWidgetSkin();

		Event_ChangeCreatorMode eventChangeCreatorMode;

	private:
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		MyGUI::Widget* getTopWidget(const MyGUI::IntPoint& _point);
		bool checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point);

		void createNewWidget();
		void moveNewWidget();
		void finishNewWidget();

		void resetWidget();
		void selectWidget();

		int toGrid(int _value);
		MyGUI::IntCoord getCoordNewWidget();

		MyGUI::IntPoint getMousePosition();

	private:
		size_t mSelectDepth;
		bool mMouseButtonPressed;
		MyGUI::IntPoint mLastClick;
		bool mCreateMode;
		std::string mWidgetType;
		std::string mWidgetSkin;
		bool mStartNewWidget;
		MyGUI::IntPoint mStartPoint;
		MyGUI::Widget* mNewWidget;
		int mGridStep;
		PositionSelectorControl* mPositionSelectorControl;
	};

} // namespace tools
*/
#endif // __WIDGET_CREATOR_MANAGER_H__
