/*!
	@file
	@author		Georgiy Evmenov
	@date		01/2009
	@module
*/
#ifndef __SAVE_LOAD_WINDOW_H__
#define __SAVE_LOAD_WINDOW_H__

#include "BaseLayout/BaseLayout.h"

class SaveLoadWindow : public wraps::BaseLayout
{
public:
	SaveLoadWindow();

	void initialise();

	void load(MyGUI::xml::ElementEnumerator _field);
	void save(MyGUI::xml::ElementPtr root);

	void setSave(std::string _fileName);
	void setLoad(std::string _fileName);

	bool isVisible() { return mMainWidget->isVisible(); }
	void setVisible(bool _visible);

	void notifyOk(MyGUI::WidgetPtr _sender = nullptr);
	void notifyCancel(MyGUI::WidgetPtr _sender = nullptr);

	typedef MyGUI::delegates::CDelegate1<const std::string &> EventInfo_OgreString;
	EventInfo_OgreString eventLoadFile;
	EventInfo_OgreString eventSaveFile;
private:
	MyGUI::ComboBoxPtr mComboFileName;
	MyGUI::ButtonPtr mButtonOk;
	MyGUI::ButtonPtr mButtonCancel;

	bool mSave;
};

#endif // __SAVE_LOAD_WINDOW_H__
