/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_Button.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	Button::Button() :
		mIsMousePressed(false),
		mIsMouseFocus(false),
		mIsStateCheck(false),
		mImage(nullptr)
	{
	}

	void Button::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
	{
		Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);

		initialiseWidgetSkin(_info);
	}

	Button::~Button()
	{
		shutdownWidgetSkin();
	}

	void Button::baseChangeWidgetSkin(ResourceSkin* _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void Button::initialiseWidgetSkin(ResourceSkin* _info)
	{
		// парсим свойства
		const MapString& properties = _info->getProperties();
		if (!properties.empty())
		{
			MapString::const_iterator iter = properties.find("ButtonPressed");
			if (iter != properties.end()) setButtonPressed(utility::parseBool(iter->second));
			iter = properties.find("StateCheck");
			if (iter != properties.end()) setStateCheck(utility::parseBool(iter->second));
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter)
		{
			if (*(*iter)->_getInternalData<std::string>() == "Image")
			{
				MYGUI_DEBUG_ASSERT( ! mImage, "widget already assigned");
				mImage = (*iter)->castType<StaticImage>();
			}
		}
	}

	void Button::shutdownWidgetSkin()
	{
		mImage = nullptr;
	}

	void Button::onMouseSetFocus(WidgetPtr _old)
	{
		_setMouseFocus(true);

		Base::onMouseSetFocus(_old);
	}

	void Button::onMouseLostFocus(WidgetPtr _new)
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

	void Button::setImageIndex(size_t _index)
	{
		if (mImage) mImage->setImageIndex(_index);
	}

	size_t Button::getImageIndex()
	{
		if (mImage) return mImage->getImageIndex();
		return ITEM_NONE;
	}

	void Button::setEnabled(bool _enabled)
	{
		if (mEnabled == _enabled) return;
		mEnabled = _enabled;

		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
		{
			(*iter)->setEnabled(_enabled);
		}

		updateButtonState();

		if ( ! mEnabled)
		{
			InputManager::getInstance().unlinkWidget(this);
			mIsMouseFocus = false;
		}
	}

	void Button::updateButtonState()
	{
		if (mIsStateCheck)
		{
			if (!mEnabled) { if (!setState("disabled_checked")) setState("disabled"); }
			else if (mIsMousePressed) { if (!setState("pushed_checked")) setState("pushed"); }
			else if (mIsMouseFocus) { if (!setState("highlighted_checked")) setState("pushed"); }
			else setState("normal_checked");
		}
		else
		{
			if (!mEnabled) setState("disabled");
			else if (mIsMousePressed) setState("pushed");
			else if (mIsMouseFocus) setState("highlighted");
			else setState("normal");
		}
	}

	void Button::setStateCheck(bool _check)
	{
		if (mIsStateCheck == _check) return;
		mIsStateCheck = _check;
		updateButtonState();
	}

	void Button::_setMouseFocus(bool _focus)
	{
		mIsMouseFocus = _focus;
		updateButtonState();
	}

	void Button::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "Button_Pressed") setButtonPressed(utility::parseValue<bool>(_value));
		else Base::setProperty(_key, _value);
	}

} // namespace MyGUI
