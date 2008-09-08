/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __FONT_PANEL_H__
#define __FONT_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout.h"

class FontPanel : public BaseLayout
{
public:
	FontPanel();

	virtual void initialise();

	void show();
	void hide();

	inline bool isShow() { return mMainWidget->isShow(); } 

private:
	void update();
	void notifyMouseButtonClick(MyGUI::WidgetPtr _widget);

private:
	MyGUI::ComboBoxPtr mComboFont;
	MyGUI::EditPtr mEditSize;
	MyGUI::EditPtr mEditResolution;
	MyGUI::ComboBoxPtr mComboAntialias;
	MyGUI::EditPtr mEditSpace;
	MyGUI::EditPtr mEditTab;
	MyGUI::EditPtr mEditCursor;
	MyGUI::EditPtr mEditDistance;
	MyGUI::EditPtr mEditOffset;
	MyGUI::ButtonPtr mButtonGenerate;
	MyGUI::EditPtr mEditDemo;
	MyGUI::StaticTextPtr mTextPix;

};

#endif // __FONT_PANEL_H__
