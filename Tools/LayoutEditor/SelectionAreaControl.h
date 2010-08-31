#ifndef __SELCTION_AREA_CONTROL_H__
#define __SELCTION_AREA_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertiesPanelView.h"
#include "WidgetsWindow.h"

namespace tools
{
	class SelectionAreaControl :
		public wraps::BaseLayout,
		public MyGUI::Singleton<SelectionAreaControl> //FIXME
	{
	public:
		SelectionAreaControl(MyGUI::Widget* _parent = nullptr);
		virtual ~SelectionAreaControl();

		MyGUI::Window* getWidgetRectangle();

	private:
		void notifyRectangleResize(MyGUI::Window* _sender);
		void notifyRectangleKeyPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

		int toGrid(int _x);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);
		void notifyChangeCoord(const MyGUI::IntCoord& _coord);

		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);

	private:
		MyGUI::Window* mCurrentWidgetRectangle;
		MyGUI::Widget* mCurrentWidget;

		int mGridStep;
		bool mArrowMove;
	};

} // namespace tools

#endif // __SELCTION_AREA_CONTROL_H__
