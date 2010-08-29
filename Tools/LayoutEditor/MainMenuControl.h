/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_MENU_CONTROL_H__
#define __MAIN_MENU_CONTROL_H__

#include <MyGUI.h>
#include "WidgetContainer.h"

namespace tools
{
	class MainMenuControl
	{
	public:
		MainMenuControl();
		virtual ~MainMenuControl();

		void setVisible(bool _value);

	private:
		void createMainMenu();
		void notifyPopupMenuAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item);

		void widgetsUpdate();
		void createWidgetPopup(WidgetContainer* _container, MyGUI::MenuCtrl* _parentPopup, bool _print_name, bool _print_type, bool _print_skin);
		void notifyWidgetsSelect(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item);
		std::string getDescriptionString(MyGUI::Widget* _widget, bool _print_name, bool _print_type, bool _print_skin);

		void notifyChangeWidgets();
		void notifySettingsChanged(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName);

		void setEdgeHideController();
		void updateRecentFilesMenu();

	private:
		MyGUI::MenuBar* mBar;
		MyGUI::MenuCtrl* mPopupMenuWidgets;
		std::vector<MyGUI::PopupMenu::ItemInfo> mWidgetMenus;
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
