/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __SETTINGS_WINDOW_H__
#define __SETTINGS_WINDOW_H__

#include "BaseLayout/BaseLayout.h"
#include "Dialog.h"

class SettingsWindow :
	public tools::Dialog
{
public:
	SettingsWindow();
	virtual ~SettingsWindow();

	MyGUI::Widget* getMainWidget() { return mMainWidget; }

protected:
	virtual void onDoModal();
	virtual void onEndModal();

private:
	int getGridStep();
	void setGridStep();

	bool getShowName() { return mCheckShowName->getStateSelected(); }
	void setShowName(bool _pressed) { mCheckShowName->setStateSelected(_pressed); }

	bool getShowType() { return mCheckShowType->getStateSelected(); }
	void setShowType(bool _pressed) { mCheckShowType->setStateSelected(_pressed); }

	bool getShowSkin() { return mCheckShowSkin->getStateSelected(); }
	void setShowSkin(bool _pressed) { mCheckShowSkin->setStateSelected(_pressed); }

	bool getEdgeHide() { return mCheckEdgeHide->getStateSelected(); }
	void setEdgeHide(bool _pressed) { mCheckEdgeHide->setStateSelected(_pressed); }

	void notifyNewGridStep(MyGUI::Widget* _sender, MyGUI::Widget* _new = 0);
	void notifyNewGridStepAccept(MyGUI::Edit* _sender); // calls previous method
	void notifyOkSettings(MyGUI::Widget* _sender);
	void notifyToggleCheck(MyGUI::Widget* _sender);

private:
	MyGUI::Edit* mGridEdit;
	MyGUI::Button* mButtonOkSettings;
	//MyGUI::ComboBox* mComboboxResolution;
	//MyGUI::ComboBox* mComboboxFullscreen;
	MyGUI::Button* mCheckShowName;
	MyGUI::Button* mCheckShowType;
	MyGUI::Button* mCheckShowSkin;
	MyGUI::Button* mCheckEdgeHide;
};

#endif // __SETTINGS_WINDOW_H__
