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
		assignWidget(mEditPrint, "edit_Print");
	}

	void FontView::setFontName(const std::string& _font)
	{
		MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
		if (!manager.isExist(_font)) return;

		unsigned int height = (manager.getByName(_font)->castType<MyGUI::IFont>())->getDefaultHeight();

		mEditView->setFontName(_font);
		mEditView->setFontHeight(height);
		mEditPrint->setFontName(_font);
		mEditPrint->setFontHeight(height);
	}

} // namespace demo
