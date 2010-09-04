#ifndef __SELCTION_AREA_CONTROL_H__
#define __SELCTION_AREA_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertiesPanelView.h"
#include "WidgetsWindow.h"

namespace tools
{
	class SelectionAreaControl :
		public wraps::BaseLayout
	{
	public:
		SelectionAreaControl(MyGUI::Widget* _parent = nullptr);
		virtual ~SelectionAreaControl();

	private:
		void notifyRectangleResize(MyGUI::Window* _sender);
		void notifyRectangleKeyPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

		int toGrid(int _x);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);
		void notifyChangeCoord(const MyGUI::IntCoord& _coord);

		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);

		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseMouseMove(MyGUI::Widget* _sender, int _left, int _top);
		void notifyMouseMouseDrag(MyGUI::Widget* _sender, int _left, int _top);

		MyGUI::Widget* getTopWidget(const MyGUI::IntPoint& _point);
		bool checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point);

	private:
		MyGUI::Window* mWindow;
		MyGUI::Widget* mCurrentWidget;

		int mGridStep;
		bool mArrowMove;
		size_t mSelectDepth;
		bool mMouseButtonPressed;
		int mLastClickX;
		int mLastClickY;
	};

} // namespace tools

#endif // __SELCTION_AREA_CONTROL_H__
