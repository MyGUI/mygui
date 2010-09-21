/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "FontView.h"

namespace demo
{

	FontView::FontView() :
		wraps::BaseLayout("FontView.layout")
	{
		assignWidget(mEditView, "edit_View");
	}

	void FontView::setFontName(const std::string& _value)
	{
		MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
		if (!manager.isExist(_value)) return;

		int height = (manager.getByName(_value)->castType<MyGUI::IFont>())->getDefaultHeight();

		mEditView->setFontName(_value);
		mEditView->setFontHeight(height);
	}

} // namespace demo
