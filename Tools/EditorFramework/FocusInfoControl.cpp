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

	FocusInfoControl::~FocusInfoControl()
	{
		MyGUI::InputManager::getInstance().eventChangeMouseFocus -=
			MyGUI::newDelegate(this, &FocusInfoControl::notifyChangeMouseFocus);
		MyGUI::InputManager::getInstance().eventChangeKeyFocus -=
			MyGUI::newDelegate(this, &FocusInfoControl::notifyChangeKeyFocus);
	}

	void FocusInfoControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view /*_layoutName*/)
	{
		Control::OnInitialise(_parent, _place, "FocusInfoControl.layout");

		assignWidget(mMouseView, "MouseView");
		assignWidget(mKeyView, "KeyView");
		mMouseView->setNeedMouseFocus(false);
		mKeyView->setNeedMouseFocus(false);

		mMouseView->setVisible(false);
		mMouseView->setCoord(MyGUI::IntCoord());
		mKeyView->setVisible(false);
		mKeyView->setCoord(MyGUI::IntCoord());

		CommandManager::getInstance()
			.getEvent("Command_FocusVisible")
			->connect(this, &FocusInfoControl::Command_FocusVisible);

		getRoot()->setVisible(SettingsManager::getInstance().getValue<bool>("Controls/FocusInfoControl/Visible"));

		MyGUI::InputManager::getInstance().eventChangeMouseFocus +=
			MyGUI::newDelegate(this, &FocusInfoControl::notifyChangeMouseFocus);
		MyGUI::InputManager::getInstance().eventChangeKeyFocus +=
			MyGUI::newDelegate(this, &FocusInfoControl::notifyChangeKeyFocus);
		mMouseFocus.eventChange += MyGUI::newDelegate(this, &FocusInfoControl::updateFocusWidgetHelpers);
		mKeyFocus.eventChange += MyGUI::newDelegate(this, &FocusInfoControl::updateFocusWidgetHelpers);
		updateFocusWidgets();
	}

	void FocusInfoControl::Command_FocusVisible(const MyGUI::UString& _commandName, bool& _result)
	{
		getRoot()->setVisible(!getRoot()->getVisible());
		SettingsManager::getInstance().setValue("Controls/FocusInfoControl/Visible", getRoot()->getVisible());

		updateFocusWidgets();

		_result = true;
	}

	void FocusInfoControl::notifyChangeMouseFocus(MyGUI::Widget* _widget)
	{
		mMouseFocus.setWidget(getRoot()->getVisible() ? _widget : nullptr);
	}

	void FocusInfoControl::notifyChangeKeyFocus(MyGUI::Widget* _widget)
	{
		mKeyFocus.setWidget(getRoot()->getVisible() ? _widget : nullptr);
	}

	void FocusInfoControl::updateFocusWidgets()
	{
		auto& input = MyGUI::InputManager::getInstance();
		notifyChangeMouseFocus(input.getMouseFocusWidget());
		notifyChangeKeyFocus(input.getKeyFocusWidget());
		updateFocusWidgetHelpers();
	}

	void FocusInfoControl::updateFocusWidgetHelpers()
	{
		MyGUI::Widget* mouse = mMouseFocus.getWidget();
		mMouseView->setVisible(getRoot()->getVisible() && mouse != nullptr);
		if (mouse != nullptr && mMouseView->getCoord() != mouse->getAbsoluteCoord())
			mMouseView->setCoord(mouse->getAbsoluteCoord());

		MyGUI::Widget* key = mKeyFocus.getWidget();
		mKeyView->setVisible(getRoot()->getVisible() && key != nullptr);
		if (key != nullptr)
		{
			MyGUI::IntCoord coord = key->getAbsoluteCoord();
			coord.left -= 1;
			coord.top -= 1;
			coord.width += 2;
			coord.height += 2;
			if (mKeyView->getCoord() != coord)
				mKeyView->setCoord(coord);
		}
	}

}
