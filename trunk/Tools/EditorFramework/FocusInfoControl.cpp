/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "FocusInfoControl.h"
#include "CommandManager.h"
#include "SettingsManager.h"

namespace tools
{

	FocusInfoControl::FocusInfoControl() :
		mMouseView(nullptr),
		mKeyView(nullptr)
	{
	}

	FocusInfoControl::~FocusInfoControl()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &FocusInfoControl::notifyFrameStart);
	}

	void FocusInfoControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, "FocusInfoControl.layout");

		assignWidget(mMouseView, "MouseView");
		assignWidget(mKeyView, "KeyView");

		mMouseView->setVisible(false);
		mMouseView->setCoord(MyGUI::IntCoord());
		mKeyView->setVisible(false);
		mKeyView->setCoord(MyGUI::IntCoord());

		CommandManager::getInstance().getEvent("Command_FocusVisible")->connect(this, &FocusInfoControl::Command_FocusVisible);

		getRoot()->setVisible(SettingsManager::getInstance().getValue<bool>("Controls/FocusInfoControl/Visible"));

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &FocusInfoControl::notifyFrameStart);
	}

	void FocusInfoControl::Command_FocusVisible(const MyGUI::UString& _commandName, bool& _result)
	{
		getRoot()->setVisible(!getRoot()->getVisible());
		SettingsManager::getInstance().setValue("Controls/FocusInfoControl/Visible", getRoot()->getVisible());

		mMouseView->setVisible(false);
		mKeyView->setVisible(false);

		_result = true;
	}

	void FocusInfoControl::notifyFrameStart(float _time)
	{
		if (getRoot()->getVisible())
			updateFocusWidgetHelpers();
	}

	void FocusInfoControl::updateFocusWidgetHelpers()
	{
		MyGUI::InputManager* input = MyGUI::InputManager::getInstancePtr();
		if (input == nullptr)
			return;

		MyGUI::Widget* mouse = input->getMouseFocusWidget();
		if (mouse != nullptr)
		{
			if (!mMouseView->getVisible())
				mMouseView->setVisible(true);

			if (mouse->getAbsoluteCoord() != mMouseCoord)
			{
				mMouseCoord = mouse->getAbsoluteCoord();
				mMouseView->setCoord(mMouseCoord);
			}
		}
		else
		{
			if (mMouseView->getVisible())
				mMouseView->setVisible(false);
		}


		MyGUI::Widget* key = input->getKeyFocusWidget();
		if (key != nullptr)
		{
			if (!mKeyView->getVisible())
				mKeyView->setVisible(true);

			if (key->getAbsoluteCoord() != mKeyCoord)
			{
				mKeyCoord = key->getAbsoluteCoord();
				mKeyView->setCoord(mKeyCoord.left - 1, mKeyCoord.top - 1, mKeyCoord.width + 2, mKeyCoord.height + 2);
			}
		}
		else
		{
			if (mKeyView->getVisible())
				mKeyView->setVisible(false);
		}
	}
}
