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
}

void FontView::update(const std::string & _font)
{
	MyGUI::FontManager & manager = MyGUI::FontManager::getInstance();
	if (!manager.isExist(_font)) return;

	int height = static_cast<MyGUI::FontPtr>(manager.getByName(_font))->getHeightPix();

	for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
		setFont((*iter), _font, height);
	}
	
}

void FontView::setFont(MyGUI::WidgetPtr _widget, const std::string & _font, int _height)
{
	_widget->setFontName(_font);
	_widget->setFontHeight(_height);
	MyGUI::EnumeratorWidgetPtr widgets = _widget->getEnumerator();
	while (widgets.next()) setFont(widgets.current(), _font, _height);
}
