/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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

	WidgetInput::~WidgetInput()
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

	bool WidgetInput::isMaskPickInside(const IntPoint& _point, const IntCoord& _coord) const
	{
		return mOwnMaskPickInfo.empty() || mOwnMaskPickInfo.pick(_point, _coord);
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

	void WidgetInput::_riseMouseDrag(int _left, int _top, MouseButton _id)
	{
		onMouseDrag(_left, _top, _id);
		eventMouseDrag(static_cast<Widget*>(this), _left, _top, _id);
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

	void WidgetInput::setNeedToolTip(bool _value)
	{
		mNeedToolTip = _value;
	}

	bool WidgetInput::getNeedToolTip() const
	{
		return mNeedToolTip;
	}

	void WidgetInput::setPointer(const std::string& _value)
	{
		mPointer = _value;
	}

	const std::string& WidgetInput::getPointer() const
	{
		return mPointer;
	}

	void WidgetInput::setNeedKeyFocus(bool _value)
	{
		mNeedKeyFocus = _value;
	}

	bool WidgetInput::getNeedKeyFocus() const
	{
		return mNeedKeyFocus;
	}

	void WidgetInput::setNeedMouseFocus(bool _value)
	{
		mNeedMouseFocus = _value;
	}

	bool WidgetInput::getNeedMouseFocus() const
	{
		return mNeedMouseFocus;
	}

	void WidgetInput::setInheritsPick(bool _value)
	{
		mInheritsPick = _value;
	}

	bool WidgetInput::getInheritsPick() const
	{
		return mInheritsPick;
	}

	bool WidgetInput::getRootMouseFocus() const
	{
		return mRootMouseFocus;
	}

	bool WidgetInput::getRootKeyFocus() const
	{
		return mRootKeyFocus;
	}

	void WidgetInput::_setRootMouseFocus(bool _value)
	{
		mRootMouseFocus = _value;
	}

	void WidgetInput::_setRootKeyFocus(bool _value)
	{
		mRootKeyFocus = _value;
	}

	void WidgetInput::onMouseLostFocus(Widget* _new)
	{
	}

	void WidgetInput::onMouseSetFocus(Widget* _old)
	{
	}

	void WidgetInput::onMouseDrag(int _left, int _top, MouseButton _id)
	{
	}

	void WidgetInput::onMouseMove(int _left, int _top)
	{
	}

	void WidgetInput::onMouseWheel(int _rel)
	{
	}

	void WidgetInput::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
	}

	void WidgetInput::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
	}

	void WidgetInput::onMouseButtonClick()
	{
	}

	void WidgetInput::onMouseButtonDoubleClick()
	{
	}

	void WidgetInput::onKeyLostFocus(Widget* _new)
	{
	}

	void WidgetInput::onKeySetFocus(Widget* _old)
	{
	}

	void WidgetInput::onKeyButtonPressed(KeyCode _key, Char _char)
	{
	}

	void WidgetInput::onKeyButtonReleased(KeyCode _key)
	{
	}

	void WidgetInput::onMouseChangeRootFocus(bool _focus)
	{
	}

	void WidgetInput::onKeyChangeRootFocus(bool _focus)
	{
	}

} // namespace MyGUI
