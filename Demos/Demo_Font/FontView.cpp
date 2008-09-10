/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "FontView.h"

FontView::FontView() :
	BaseLayout("FontView.layout")
{
}

void FontView::initialise()
{
	loadLayout();
	update();
}

void FontView::update()
{
	MyGUI::FontManager & manager = MyGUI::FontManager::getInstance();
	if (!manager.isExist("font_tmp")) return;

	int height = static_cast<MyGUI::FontPtr>(manager.getByName("font_tmp"))->getHeightPix();

	for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
		setFont((*iter), height);
	}
	
}

void FontView::setFont(MyGUI::WidgetPtr _widget, int _height)
{
	_widget->setFontName("font_tmp");
	_widget->setFontHeight(_height);
	MyGUI::EnumeratorWidgetPtr widgets = _widget->getEnumerator();
	while (widgets.next()) setFont(widgets.current(), _height);
}
