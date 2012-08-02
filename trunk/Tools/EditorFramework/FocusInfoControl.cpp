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
		mWidgetKeyFocus(nullptr),
		mWidgetMouseFocus(nullptr),
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
		mKeyView->setVisible(false);

		CommandManager::getInstance().getEvent("Command_FocusVisible")->connect(this, &FocusInfoControl::Command_FocusVisible);

		getRoot()->setVisible(SettingsManager::getInstance().getValue<bool>("Controls/FocusInfoControl/Visible"));

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &FocusInfoControl::notifyFrameStart);
	}

	void FocusInfoControl::Command_FocusVisible(const MyGUI::UString& _commandName, bool& _result)
	{
		getRoot()->setVisible(!getRoot()->getVisible());
		SettingsManager::getInstance().setValue("Controls/FocusInfoControl/Visible", getRoot()->getVisible());

		mWidgetKeyFocus = nullptr;
		mWidgetMouseFocus = nullptr;
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
		if (mouse != mWidgetMouseFocus)
		{
			mWidgetMouseFocus = mouse;

			if (mWidgetMouseFocus == nullptr)
			{
				mMouseView->setVisible(false);
			}
			else
			{
				mMouseView->setVisible(true);
				mMouseView->setCoord(mWidgetMouseFocus->getAbsoluteCoord());
			}
		}

		MyGUI::Widget* key = input->getKeyFocusWidget();
		if (key != mWidgetKeyFocus)
		{
			mWidgetKeyFocus = key;

			if (mWidgetKeyFocus == nullptr)
			{
				mKeyView->setVisible(false);
			}
			else
			{
				mKeyView->setVisible(true);
				mKeyView->setCoord(mWidgetKeyFocus->getAbsoluteCoord());
			}
		}
	}
}
