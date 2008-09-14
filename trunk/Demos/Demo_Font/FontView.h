/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __FONT_VIEW_H__
#define __FONT_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout.h"

class FontView : public BaseLayout
{
public:
	FontView();
	virtual void initialise();

	void update(const std::string & _font);

private:
	void setFont(MyGUI::WidgetPtr _widget, const std::string & _font, int _height);
};

#endif // __FONT_VIEW_H__
