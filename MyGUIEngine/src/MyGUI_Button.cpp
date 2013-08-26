/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Button.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_ImageBox.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	Button::Button() :
		mIsMousePressed(false),
		mIsMouseFocus(false),
		mStateSelected(false),
		mImage(nullptr),
		mModeImage(false)
	{
	}

	void Button::initialiseOverride()
	{
		Base::initialiseOverride();

		///@wskin_child{Button, ImageBox, Image} Картинка, в режиме картинки отображает состояние кнопки.
		assignWidget(mImage, "Image");
	}

	void Button::shutdownOverride()
	{
		mImage = nullptr;

		Base::shutdownOverride();
	}

	void Button::onMouseSetFocus(Widget* _old)
	{
		_setMouseFocus(true);

		Base::onMouseSetFocus(_old);
	}

	void Button::onMouseLostFocus(Widget* _new)
	{
		_setMouseFocus(false);

		Base::onMouseLostFocus(_new);
	}

	void Button::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		if (_id == MouseButton::Left)
		{
			mIsMousePressed = true;
			updateButtonState();
		}

		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void Button::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		if (_id == MouseButton::Left)
		{
			mIsMousePressed = false;
			updateButtonState();
		}

		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void Button::updateButtonState()
	{
		if (mStateSelected)
		{
			if (!getInheritedEnabled())
			{
				if (!_setState("disabled_checked"))
					_setState("disabled");
			}
			else if (mIsMousePressed)
			{
				if (!_setState("pushed_checked"))
					_setState("pushed");
			}
			else if (mIsMouseFocus)
			{
				if (!_setState("highlighted_checked"))
					_setState("pushed");
			}
			else
				_setState("normal_checked");
		}
		else
		{
			if (!getInheritedEnabled())
				_setState("disabled");
			else if (mIsMousePressed)
				_setState("pushed");
			else if (mIsMouseFocus)
				_setState("highlighted");
			else
				_setState("normal");
		}
	}

	void Button::setStateSelected(bool _check)
	{
		if (mStateSelected == _check)
			return;

		mStateSelected = _check;
		updateButtonState();
	}

	void Button::_setMouseFocus(bool _focus)
	{
		mIsMouseFocus = _focus;
		updateButtonState();
	}

	void Button::baseUpdateEnable()
	{
		updateButtonState();
		if (!getInheritedEnabled())
		{
			mIsMouseFocus = false;
		}
	}

	void Button::setModeImage(bool _value)
	{
		mModeImage = _value;
		updateButtonState();
	}

	bool Button::_setState(const std::string& _value)
	{
		if (mModeImage)
		{
			if (mImage)
				mImage->setItemName(_value);

			_setWidgetState(_value);
			return true;
		}

		return _setWidgetState(_value);
	}

	void Button::setImageResource(const std::string& _name)
	{
		if (mImage)
			mImage->setItemResource(_name);
		updateButtonState();
	}

	void Button::setImageGroup(const std::string& _name)
	{
		if (mImage)
			mImage->setItemGroup(_name);
		updateButtonState();
	}

	void Button::setImageName(const std::string& _name)
	{
		if (mImage)
			mImage->setItemName(_name);
	}

	void Button::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{Button, StateSelected, bool} Set state selected.
		if (_key == "StateSelected")
			setStateSelected(utility::parseValue<bool>(_value));

		/// @wproperty{Button, ModeImage, bool} Устанавливает режим работы кнопки, в котором она свои состояния берет из картинки.
		else if (_key == "ModeImage")
			setModeImage(utility::parseValue<bool>(_value));

		/// @wproperty{Button, ImageResource, string} Ссылка на ресурс картинки.
		else if (_key == "ImageResource")
			setImageResource(_value);

		/// @wproperty{Button, ImageGroup, string} Группа картинки в ресурсе.
		else if (_key == "ImageGroup")
			setImageGroup(_value);

		/// @wproperty{Button, ImageName, string} Имя картинки в ресурсе.
		else if (_key == "ImageName")
			setImageName(_value);

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	bool Button::getStateSelected() const
	{
		return mStateSelected;
	}

	bool Button::getModeImage() const
	{
		return mModeImage;
	}

	ImageBox* Button::_getImageBox()
	{
		return mImage;
	}

} // namespace MyGUI
