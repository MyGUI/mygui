/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "FontView.h"

namespace demo
{

	FontView::FontView() : wraps::BaseLayout("FontView.layout")
	{
		assignWidget(mEditView, "edit_View");
	}

	void FontView::setFontName(const std::string & _font)
	{
		MyGUI::FontManager & manager = MyGUI::FontManager::getInstance();
		if (!manager.isExist(_font)) return;

		unsigned int height = static_cast<MyGUI::FontPtr>(manager.getByName(_font))->getHeightPix();

		mEditView->setFontName(_font);
		mEditView->setFontHeight(height);
	}

} // namespace demo
