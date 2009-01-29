/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
	@module
*/

#include "precompiled.h"
#include "EditorToolTip.h"
#include "MyGUI_SkinManager.h"

EditorToolTip::EditorToolTip() : BaseLayout("EditorToolTip.layout")
{
	assignWidget(mText, "Text");

	minWidth = MyGUI::utility::parseInt(mMainWidget->getUserString("minWidth"));
	minHeight = MyGUI::utility::parseInt(mMainWidget->getUserString("minHeight"));
	lastWidget = nullptr;
}

void EditorToolTip::show(const Ogre::UTFString & _text, const MyGUI::IntPoint & _point)
{
	if (_text.empty()) return;

	setPosition(_point);
	mText->setCaption(MyGUI::LanguageManager::getInstance().replaceTags(_text));
	mMainWidget->setVisible(true);
}

void EditorToolTip::show(MyGUI::WidgetPtr _sender, const MyGUI::IntPoint & _point)
{
	std::string widget = _sender->getUserString("widget");
	std::string skin = _sender->getUserString("skin");
	std::string text = "Widget: " + widget +
		"\nSkin: " + skin +
		"\nDefaultSize: " + _sender->getUserString("width") + " x " + _sender->getUserString("height")
		;

	mText->setCaption(text);

	int width = MyGUI::utility::parseInt(_sender->getUserString("width"));
	int height = MyGUI::utility::parseInt(_sender->getUserString("height"));

	const int MARGIN = 5;
	const int LINE_HEIGHT = 22;
	const int LINES = 3;

	// óçíàåì ðàçìåð ñêèíà, òàê ÷òîáû äî÷êè áûëè ïîëíîðàçìåðíûå
	MyGUI::SkinManager & manager = MyGUI::SkinManager::getInstance();
	if (manager.isExist(skin)) {
		// ìàêñèìàëüíàÿ ðàçíèöà
		MyGUI::IntSize max_size;

		MyGUI::WidgetSkinInfo * info = manager.getSkin(skin);
		if (info != nullptr) {
			const MyGUI::VectorChildSkinInfo & child = info->getChild();
			for (size_t pos=0; pos<child.size(); ++pos) {
				const std::string & child_skin = child[pos].skin;
				if (!manager.isExist(child_skin)) continue;
				const MyGUI::WidgetSkinInfo * child_info = manager.getSkin(child_skin);
				const MyGUI::IntSize & child_size = child[pos].coord.size();
				MyGUI::IntSize dif_size = child_info->getSize() - child_size;

				if (max_size.width < dif_size.width) max_size.width = dif_size.width;
				if (max_size.height < dif_size.height) max_size.height = dif_size.height;
			}
		}

		// ïðèáàâëÿåì ðàçìåð äåòåé
		width += max_size.width;
		height += max_size.height;
	}

	mMainWidget->setSize(std::max(minWidth, width + 2*MARGIN), std::max(minHeight, height + LINE_HEIGHT*LINES + 2*MARGIN));
	if (lastWidget) MyGUI::Gui::getInstance().destroyWidget(lastWidget);
	lastWidget = mMainWidget->createWidgetT("Widget", skin, MARGIN, MARGIN + LINE_HEIGHT*LINES, width, height, MyGUI::Align::Default);
	lastWidget->setCaption(skin);

	setPosition(_point);
	mMainWidget->setVisible(true);
}

void EditorToolTip::hide()
{
	mMainWidget->setVisible(false);
}

void EditorToolTip::setPosition(const MyGUI::IntPoint & _point)
{
	const MyGUI::IntPoint offset(10, 10);

	MyGUI::IntPoint point = _point + offset;
	MyGUI::Gui * gui = MyGUI::Gui::getInstancePtr();

	const MyGUI::IntSize & size = mMainWidget->getSize();

	if ((point.left + size.width) > gui->getViewWidth()) {
		point.left -= offset.left + offset.left + size.width;
	}
	if ((point.top + size.height) > gui->getViewHeight()) {
		point.top -= offset.top + offset.top + size.height;
	}

	mMainWidget->setPosition(point);
}
