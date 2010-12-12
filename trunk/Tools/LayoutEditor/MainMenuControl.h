/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_MENU_CONTROL_H__
#define __MAIN_MENU_CONTROL_H__

#include <MyGUI.h>
#include "WidgetContainer.h"
#include "BaseLayout/BaseLayout.h"

namespace tools
{
	class MainMenuControl :
		public wraps::BaseLayout
	{
	public:
		MainMenuControl(MyGUI::Widget* _parent = nullptr);
		virtual ~MainMenuControl();

		void setVisible(bool _value);

	private:
		void createMainMenu();
		void notifyPopupMenuAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item);

		void widgetsUpdate();
		void createWidgetPopup(WidgetContainer* _container, MyGUI::MenuControl* _parentPopup, bool _print_name, bool _print_type, bool _print_skin);
		void notifyWidgetsSelect(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item);
		std::string getDescriptionString(MyGUI::Widget* _widget, bool _print_name, bool _print_type, bool _print_skin);

		void notifyChangeWidgets();
		void notifySettingsChanged(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName);

		void updateRecentFilesMenu();
		void updateRecentProjectsMenu();

	private:
		MyGUI::MenuBar* mBar;
		MyGUI::MenuControl* mPopupMenuWidgets;
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
