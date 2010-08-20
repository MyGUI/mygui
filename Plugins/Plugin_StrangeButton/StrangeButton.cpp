/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#include "StrangeButton.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_ISubWidgetText.h"

namespace plugin
{

	StrangeButton::StrangeButton() :
		mIsMousePressed(false),
		mIsMouseFocus(false),
		mIsStateCheck(false)
	{
	}

	void StrangeButton::initialiseOverride()
	{
		Base::initialiseOverride();
	}

	void StrangeButton::shutdownOverride()
	{
		Base::shutdownOverride();
	}

	void StrangeButton::onMouseSetFocus(MyGUI::Widget* _old)
	{
		Base::onMouseSetFocus(_old);
		mIsMouseFocus = true;

		if (getSubWidgetText() == nullptr)
			return;
		std::string s, str = getSubWidgetText()->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];

		getSubWidgetText()->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::onMouseLostFocus(MyGUI::Widget* _new)
	{
		Base::onMouseLostFocus(_new);
		mIsMouseFocus = false;

		if (getSubWidgetText() == nullptr)
			return;

		std::string s, str = getSubWidgetText()->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];
		
		getSubWidgetText()->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			mIsMousePressed = true;
			updateButtonState();
		}

		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void StrangeButton::onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			mIsMousePressed = false;
			updateButtonState();
		}

		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void StrangeButton::updateButtonState()
	{
		if (mIsStateCheck)
		{
			if (!getEnabled())
			{
				if (!setState("disabled_checked"))
					setState("disabled");
			}
			else if (mIsMousePressed)
			{
				if (!setState("pushed_checked"))
					setState("pushed");
			}
			else if (mIsMouseFocus)
			{
				if (!setState("highlighted_checked"))
					setState("pushed");
			}
			else
				setState("normal_checked");
		}
		else
		{
			if (!getEnabled())
				setState("disabled");
			else if (mIsMousePressed)
				setState("pushed");
			else if (mIsMouseFocus)
				setState("highlighted");
			else
				setState("normal");
		}
	}

	void StrangeButton::setStateCheck(bool _check)
	{
		if (mIsStateCheck == _check) return;
		mIsStateCheck = _check;
		updateButtonState();
	}

	void StrangeButton::_setMouseFocus(bool _focus)
	{
		mIsMouseFocus = _focus;
		updateButtonState();
	}

	void StrangeButton::baseUpdateEnable()
	{
		updateButtonState();
		if (!getEnabled())
		{
			mIsMouseFocus = false;
		}
	}

} // namespace plugin