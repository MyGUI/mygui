/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __SETTINGS_WINDOW_H__
#define __SETTINGS_WINDOW_H__

#include "BaseLayout/BaseLayout.h"

class SettingsWindow : public wraps::BaseLayout
{
public:
	SettingsWindow();

	void load(MyGUI::xml::ElementEnumerator field);
	void save(MyGUI::xml::ElementPtr root);

	int getGridStep();
	void setGridStep();

	bool getShowName(){return mCheckShowName->getButtonPressed();};
	bool getShowType(){return mCheckShowType->getButtonPressed();};
	bool getShowSkin(){return mCheckShowSkin->getButtonPressed();};
	bool getEdgeHide(){return mCheckEdgeHide->getButtonPressed();};
	void setShowName(bool _pressed){mCheckShowName->setButtonPressed(_pressed);};
	void setShowType(bool _pressed){mCheckShowType->setButtonPressed(_pressed);};
	void setShowSkin(bool _pressed){mCheckShowSkin->setButtonPressed(_pressed);};
	void setEdgeHide(bool _pressed){mCheckEdgeHide->setButtonPressed(_pressed);};

	typedef MyGUI::delegates::CDelegate0 EventInfo;
	EventInfo eventWidgetsUpdate;

	MyGUI::WidgetPtr getMainWidget() { return mMainWidget; }
	void setVisible(bool _visible) { mMainWidget->setVisible(_visible); }

private:
	void notifyNewGridStep(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new = 0);
	void notifyNewGridStepAccept(MyGUI::EditPtr _sender); // calls previous method
	void notifyOkSettings(MyGUI::WidgetPtr _sender);
	void notifyToggleCheck(MyGUI::WidgetPtr _sender);
private:
	MyGUI::EditPtr mGridEdit;
	MyGUI::ButtonPtr mButtonOkSettings;
	MyGUI::ComboBoxPtr mComboboxResolution;
	MyGUI::ComboBoxPtr mComboboxFullscreen;
	MyGUI::ButtonPtr mCheckShowName;
	MyGUI::ButtonPtr mCheckShowType;
	MyGUI::ButtonPtr mCheckShowSkin;
	MyGUI::ButtonPtr mCheckEdgeHide;
};

#endif // __SETTINGS_WINDOW_H__
