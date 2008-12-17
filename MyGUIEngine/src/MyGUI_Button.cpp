/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Button.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	Button::Button(WidgetType _behaviour, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Widget(_behaviour, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mIsMousePressed(false),
		mIsMouseFocus(false),
		mIsStateCheck(false),
		mImage(null)
	{
		initialiseWidgetSkin(_info);
	}

	Button::~Button()
	{
		shutdownWidgetSkin();
	}

	void Button::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void Button::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// парсим свойства
		const MapString & properties = _info->getProperties();
		if (!properties.empty()) {
			MapString::const_iterator iter = properties.find("ButtonPressed");
			if (iter != properties.end()) setButtonPressed(utility::parseBool(iter->second));
			iter = properties.find("StateCheck");
			if (iter != properties.end()) setStateCheck(utility::parseBool(iter->second));
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "Image") {
				MYGUI_DEBUG_ASSERT( ! mImage, "widget already assigned");
				mImage = (*iter)->castType<StaticImage>();
			}
		}
	}

	void Button::shutdownWidgetSkin()
	{
		mImage = null;
	}

	void Button::onMouseSetFocus(WidgetPtr _old)
	{
		_setMouseFocus(true);
		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseSetFocus(_old);
	}

	void Button::onMouseLostFocus(WidgetPtr _new)
	{
		_setMouseFocus(false);
		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseLostFocus(_new);
	}

	void Button::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		if (_id == MB_Left) {
			mIsMousePressed = true;
			updateButtonState();
		}
		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseButtonPressed(_left, _top, _id);
	}

	void Button::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		if (_id == MB_Left) {
			mIsMousePressed = false;
			updateButtonState();
		}
		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseButtonReleased(_left, _top, _id);
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

		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter) {
			(*iter)->setEnabled(_enabled);
		}

		updateButtonState();

		if ( ! mEnabled) {
			InputManager::getInstance()._unlinkWidget(this);
			mIsMouseFocus = false;
		}
	}

	void Button::updateButtonState()
	{
		if (mIsStateCheck) {
			if (!mEnabled) { if (!setState("disabled_checked")) setState("disabled"); }
			else if (mIsMousePressed) { if (!setState("pushed_checked")) setState("pushed"); }
			else if (mIsMouseFocus) { if (!setState("highlighted_checked")) setState("pushed"); }
			else setState("normal_checked");
		}
		else {
			if (!mEnabled) setState("disabled");
			else if (mIsMousePressed) setState("pushed");
			else if (mIsMouseFocus) setState("highlighted");
			else setState("normal");
		}
	}

} // namespace MyGUI
