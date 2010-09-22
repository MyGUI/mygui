/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#include "Precompiled.h"
#include "InformationWindow.h"

namespace demo
{

	InformationWindow::InformationWindow(MyGUI::Widget* _parent) :
		BaseLayout("InformationWindow.layout", _parent),
		mFocus(nullptr)
	{
		assignWidget(mInfo, "Info");

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &InformationWindow::notifyFrameStart);

		if (_parent)
		{
			const MyGUI::IntCoord& coord = _parent->getClientCoord();
			const MyGUI::IntSize& size = mMainWidget->getSize();
			mMainWidget->setPosition(MyGUI::IntPoint(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height)));
		}
	}

	InformationWindow::~InformationWindow()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &InformationWindow::notifyFrameStart);
	}

	int InformationWindow::getRand(int _min, int _max)
	{
		if (_max < _min)
			std::swap(_max, _min);
		int range = _max - _min;
		if (range == 0)
			return 0;
		int result = ::rand() % range;
		if (result < 0)
			result = -result;
		return _min + result;
	}

	void InformationWindow::notifyFrameStart(float _time)
	{
		MyGUI::Widget* focus = MyGUI::InputManager::getInstance().getMouseFocusWidget();
		if (focus == mFocus)
			return;
		mFocus = focus;

		const std::string tag = "Info";
		std::string info;
		if (mFocus != nullptr)
		{
			while (true)
			{
				if (focus->isUserString(tag))
				{
					info = focus->getUserString(tag);
					break;
				}
				if (!focus->getParent())
				{
					break;
				}
				focus = focus->getParent();
			}
		}

		mInfo->setCaption(info);
	}

} // namespace demo
