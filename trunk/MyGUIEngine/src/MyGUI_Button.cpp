/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Button.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_CastWidget.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	Ogre::String Button::WidgetTypeName = "Button";

	Button::Button(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mIsPressed(false),
		mIsFocus(false),
		mIsStatePressed(false),
		mImage(null)
	{

		// ïàðñèì ñâîéñòâà
		const MapString & param = _info->getParams();
		if (!param.empty()) {
			MapString::const_iterator iter = param.find("ButtonPressed");
			if (iter != param.end()) setButtonPressed(iter->second == "true");
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Image") {
				mImage = castWidget<StaticImage>(*iter);
			}
		}
	}

	void Button::_onMouseSetFocus(WidgetPtr _old)
	{
		_setMouseFocus(true);
		// !!! ÎÁßÇÀÒÅËÜÍÎ âûçûâàòü â êîíöå ìåòîäà
		Widget::_onMouseSetFocus(_old);
	}

	void Button::_onMouseLostFocus(WidgetPtr _new)
	{
		_setMouseFocus(false);
		// !!! ÎÁßÇÀÒÅËÜÍÎ âûçûâàòü â êîíöå ìåòîäà
		Widget::_onMouseLostFocus(_new);
	}

	void Button::_onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		if (_id == MB_Left) {
			mIsPressed = true;
			updateButtonState();
		}
		// !!! ÎÁßÇÀÒÅËÜÍÎ âûçûâàòü â êîíöå ìåòîäà
		Widget::_onMouseButtonPressed(_left, _top, _id);
	}

	void Button::_onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		if (_id == MB_Left) {
			mIsPressed = false;
			updateButtonState();
		}
		// !!! ÎÁßÇÀÒÅËÜÍÎ âûçûâàòü â êîíöå ìåòîäà
		Widget::_onMouseButtonReleased(_left, _top, _id);
	}

	void Button::setImageIndex(size_t _index)
	{
		if (mImage) mImage->setImageNum(_index);
	}

	size_t Button::getImageIndex()
	{
		if (mImage) return mImage->getImageNum();
		return ITEM_NONE;
	}

	void Button::setEnabled(bool _enabled)
	{
		mEnabled = _enabled;

		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter) {
			(*iter)->setEnabled(_enabled);
		}

		updateButtonState();

		if ( ! mEnabled) InputManager::getInstance()._unlinkWidget(this);
	}

	void Button::updateButtonState()
	{
		if ( ! mEnabled) {
			if (mIsStatePressed) setState("disable_pressed");
			else setState("disable");
		}
		else {
			if (mIsFocus) {
				if (mIsPressed || mIsStatePressed) setState("select");
				else setState("active");
			} else {
				if (mIsPressed || mIsStatePressed) setState("pressed");
				else setState("normal");
			}
		}
	}

} // namespace MyGUI
