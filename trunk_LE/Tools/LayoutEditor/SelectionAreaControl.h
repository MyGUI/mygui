#ifndef __SELCTION_AREA_CONTROL_H__
#define __SELCTION_AREA_CONTROL_H__

/*#include "BaseLayout/BaseLayout.h"
#include "PropertiesPanelView.h"

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

		int toGrid(int _value, MyGUI::Widget* _widget, bool _horizont);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);
		void notifyChangeCoord(const MyGUI::IntCoord& _coord);

		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);

		void notifyMouseMouseMove(MyGUI::Widget* _sender, int _left, int _top);
		void notifyMouseMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

	private:
		MyGUI::Window* mWindow;
		MyGUI::Widget* mCurrentWidget;

		int mGridStep;
		bool mArrowMove;
	};

} // namespace tools
*/
#endif // __SELCTION_AREA_CONTROL_H__
