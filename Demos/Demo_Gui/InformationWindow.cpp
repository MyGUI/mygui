/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

#include "InformationWindow.h"

namespace demo
{
	int InformationWindow::getRand(int _min, int _max)
	{
		if (_max < _min) std::swap(_max, _min);
		int range = _max - _min;
		if (range == 0) return 0;
		int result = ::rand() % range;
		if (result < 0) result = -result;
		return _min + result;
	}

	InformationWindow::InformationWindow() :
		BaseLayout("InformationWindow.layout"),
		mFocus(null)
	{
	}

	void InformationWindow::initialise(MyGUI::WidgetPtr _parent)
	{
		loadLayout(_parent);

		assignWidget(mInfo, "Info");

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &InformationWindow::notifyFrameStart);

		if (_parent) {
			const MyGUI::IntCoord& coord = _parent->getClientCoord();
			const MyGUI::IntSize& size = mMainWidget->getSize();
			mMainWidget->setPosition(MyGUI::IntPoint(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height)));
		}

	}

	void InformationWindow::shutdown()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &InformationWindow::notifyFrameStart);
		wraps::BaseLayout::shutdown();
	}

	void InformationWindow::notifyFrameStart(float _time)
	{
		MyGUI::WidgetPtr focus = MyGUI::InputManager::getInstance().getMouseFocusWidget();
		if (focus == mFocus) return;
		mFocus = focus;

		const std::string tag = "Info";
		std::string info;
		if (mFocus != null) {
			while (true) {
				if (focus->isUserString(tag)) {
					info = focus->getUserString(tag);
					break;
				}
				if (!focus->getParent()) {
					break;
				}
				focus = focus->getParent();
			}
		}

		mInfo->setCaption(info);
	}

} // namespace demo
