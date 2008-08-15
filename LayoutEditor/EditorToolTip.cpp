/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
	@module
*/

#include "EditorToolTip.h"

EditorToolTip::EditorToolTip() :
	BaseLayout("EditorToolTip.layout")
{
}

void EditorToolTip::initialise()
{
	loadLayout();

	assignWidget(mText, "Text");
}

void EditorToolTip::show(const Ogre::UTFString & _text, const MyGUI::IntPoint & _point)
{
	const MyGUI::IntPoint offset(10, 10);

	if (_text.empty()) return;

	MyGUI::IntPoint point = _point + offset;
	MyGUI::Gui * gui = MyGUI::Gui::getInstancePtr();

	const MyGUI::IntSize & size = mMainWidget->getSize();

	if ((point.left + size.width) > gui->getViewWidth()) {
		point.left -= offset.left + offset.left + size.width;
	}
	if ((point.top + size.height) > gui->getViewHeight()) {
		point.top -= offset.top + offset.top + size.height;
	}

	mText->setCaption(MyGUI::LanguageManager::getInstance().replaceTags(_text));

	mMainWidget->setPosition(point);
	mMainWidget->show();
}

void EditorToolTip::hide()
{
	mMainWidget->hide();
}
