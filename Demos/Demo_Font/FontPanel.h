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

};

#endif // __FONT_PANEL_H__
