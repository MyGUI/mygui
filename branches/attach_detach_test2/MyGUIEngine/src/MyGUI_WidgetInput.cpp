/*!
	@file
	@author		Albert Semenov
	@date		05/2010
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
#include "MyGUI_WidgetInput.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	WidgetInput::WidgetInput() :
		mNeedToolTip(false),
		mInheritsPick(false),
		mNeedKeyFocus(false),
		mNeedMouseFocus(true),
		mRootMouseFocus(false),
		mRootKeyFocus(false)
	{
	}

	void WidgetInput::setMaskPick(const std::string& _filename)
	{
		if (_filename.empty())
			mOwnMaskPickInfo = MaskPickInfo();
		else if (!mOwnMaskPickInfo.load(_filename))
			MYGUI_LOG(Error, "mask not load '" << _filename << "'");
	}

	void WidgetInput::setMaskPick(const MaskPickInfo& _info)
	{
		mOwnMaskPickInfo = _info;
	}

	bool WidgetInput::isMaskPickInside(const IntPoint& _point, const IntCoord& _coord)
	{
		return !mOwnMaskPickInfo.empty() && mOwnMaskPickInfo.pick(_point, _coord);
	}

	void WidgetInput::_riseMouseLostFocus(Widget* _new)
	{
		onMouseLostFocus(_new);
		eventMouseLostFocus(static_cast<Widget*>(this), _new);
	}

	void WidgetInput::_riseMouseSetFocus(Widget* _old)
	{
		onMouseSetFocus(_old);
		eventMouseSetFocus(static_cast<Widget*>(this), _old);
	}

	void WidgetInput::_riseMouseDrag(int _left, int _top)
	{
		onMouseDrag(_left, _top);
		eventMouseDrag(static_cast<Widget*>(this), _left, _top);
	}

	void WidgetInput::_riseMouseMove(int _left, int _top)
	{
		onMouseMove(_left, _top);
		eventMouseMove(static_cast<Widget*>(this), _left, _top);
	}

	void WidgetInput::_riseMouseWheel(int _rel)
	{
		onMouseWheel(_rel);
		eventMouseWheel(static_cast<Widget*>(this), _rel);
	}

	void WidgetInput::_riseMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		onMouseButtonPressed(_left, _top, _id);
		eventMouseButtonPressed(static_cast<Widget*>(this), _left, _top, _id);
	}

	void WidgetInput::_riseMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		onMouseButtonReleased(_left, _top, _id);
		eventMouseButtonReleased(static_cast<Widget*>(this), _left, _top, _id);
	}

	void WidgetInput::_riseMouseButtonClick()
	{
		onMouseButtonClick();
		eventMouseButtonClick(static_cast<Widget*>(this));
	}

	void WidgetInput::_riseMouseButtonDoubleClick()
	{
		onMouseButtonDoubleClick();
		eventMouseButtonDoubleClick(static_cast<Widget*>(this));
	}

	void WidgetInput::_riseKeyLostFocus(Widget* _new)
	{
		onKeyLostFocus(_new);
		eventKeyLostFocus(static_cast<Widget*>(this), _new);
	}

	void WidgetInput::_riseKeySetFocus(Widget* _old)
	{
		onKeySetFocus(_old);
		eventKeySetFocus(static_cast<Widget*>(this), _old);
	}

	void WidgetInput::_riseKeyButtonPressed(KeyCode _key, Char _char)
	{
		onKeyButtonPressed(_key, _char);
		eventKeyButtonPressed(static_cast<Widget*>(this), _key, _char);
	}

	void WidgetInput::_riseKeyButtonReleased(KeyCode _key)
	{
		onKeyButtonReleased(_key);
		eventKeyButtonReleased(static_cast<Widget*>(this), _key);
	}

	void WidgetInput::_riseMouseChangeRootFocus(bool _focus)
	{
		onMouseChangeRootFocus(_focus);
		eventRootMouseChangeFocus(static_cast<Widget*>(this), _focus);
	}

	void WidgetInput::_riseKeyChangeRootFocus(bool _focus)
	{
		onKeyChangeRootFocus(_focus);
		eventRootKeyChangeFocus(static_cast<Widget*>(this), _focus);
	}

} // namespace MyGUI
