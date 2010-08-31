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
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);
		void notifyChangeSelectedWidget(MyGUI::Widget* _widget);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		int toGrid(int _x);

	private:
		MyGUI::Window* mWindow;
		int mGridStep;
		MyGUI::Widget* mCurrentWidget;
		bool mArrowMove;
	};

} // namespace tools

#endif // __SELCTION_AREA_CONTROL_H__
