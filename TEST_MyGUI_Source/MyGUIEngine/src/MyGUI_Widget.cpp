/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Widget.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_LayerKeeper.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_SubWidgetTextInterface.h"

namespace MyGUI
{

	Widget::Widget(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		CroppedRectangleInterface(IntCoord(_coord.point(), _info->getSize()), _align, _parent), // ������ �� �����
		mOwner(static_cast<Widget*>(_parent)),
		UserData(),
		mStateInfo(_info->getStateInfo()),
		mMaskPeekInfo(_info->getMask()),
		mText(null),
		mEnabled(true),
		mVisible(true),
		mAlpha(ALPHA_MIN),
		mName(_name),
		mLayerKeeper(null),
		mLayerItemKeeper(null),
		mTexture(_info->getTextureName())
	{
		// ������������ ���������� ����������
		mAbsolutePosition = _coord.point();
		if (null != mParent) mAbsolutePosition += mParent->getAbsolutePosition();

		// ��� ���������� ���������
		mWidgetEventSender = this;

		// ��������� ��������� �������
		SubWidgetManager & manager = SubWidgetManager::getInstance();
		for (VectorSubWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); ++iter) {
			CroppedRectangleInterface * sub = manager.createSubWidget(*iter, this);
			mSubSkinChild.push_back(sub);
			if (sub->_isText()) mText = static_cast<SubWidgetTextInterfacePtr>(mText);
		}

		// ������ ��������
		const MapString & param = _info->getParams();
		if (false == param.empty()) {
			MapString::const_iterator iter = param.find("FontName");
			if (iter != param.end()) setFontName(iter->second);
			iter = param.find("FontHeight");
			if (iter != param.end()) setFontHeight(utility::parseInt(iter->second));
			iter = param.find("NeedKey");
			if (iter != param.end()) setNeedKeyFocus(iter->second == "true");
			iter = param.find("AlignText");
			if (iter != param.end()) setTextAlign(SkinManager::getInstance().parseAlign(iter->second));
			iter = param.find("Colour");
			if (iter != param.end()) setColour(utility::parseColour(iter->second));
			iter = param.find("Pointer");
			if (iter != param.end()) mPointer = iter->second;
			iter = param.find("Show");
			if (iter != param.end()) {
				if (utility::parseBool(iter->second)) show();
				else hide();
			}
		}

		// ���� ����� ���� ������, ���� ���� ��������� �� ����� ���
		setState("normal");
		setAlpha(ALPHA_MAX);
		// ����� ����
		//if ( (mParent != null) && (static_cast<WidgetPtr>(mParent)->getAlpha() != ALPHA_MAX) ) setAlpha(static_cast<WidgetPtr>(mParent)->getAlpha());

		// ������� �����
		const VectorChildSkinInfo& child = _info->getChild();
		for (VectorChildSkinInfo::const_iterator iter=child.begin(); iter!=child.end(); ++iter) {
			WidgetPtr widget = createWidgetT(iter->type, iter->skin, iter->coord, iter->align);
			widget->_setInternalString(iter->name);
			// ��������� UserString ����������
			for (MapString::const_iterator prop=iter->params.begin(); prop!=iter->params.end(); ++prop) {
				widget->setUserString(prop->first, prop->second);
			}
		}

		// � ��� ������ ���������� ������
		setSize(_coord.size());

		// ���� ���� ��� ����������, �� � �� ���������
		if ((null != mParent) && (null != getParent()->getLayerItemKeeper())) _attachToLayerItemKeeper(getParent()->getLayerItemKeeper());
	}

	Widget::~Widget()
	{
		if (null != mLayerItemKeeper) _detachFromLayerItemKeeper();

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			delete (*skin);
		}

		mSubSkinChild.clear();
		_destroyAllChildWidget();
	}

	WidgetPtr Widget::createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _coord, _align, this, _name);
		mWidgetChild.push_back(widget);
		return widget;
	}

	WidgetPtr Widget::createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _name)
	{
		return createWidgetT(_type, _skin, Gui::getInstance().convertRelativeToInt(_coord, this), _align, _name);
	}

	void Widget::_setVisible(bool _visible)
	{
		if (mVisible == _visible) return;
		mVisible = _visible;

		// ���� ������ �������������, �� �� ����������
		if (mVisible && !mShow) return;

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) {
			(*widget)->_setVisible(mVisible);
		}

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			mVisible ? (*skin)->show() : (*skin)->hide();
		}
	}

	void Widget::show()
	{
		if (mShow) return;
		mShow = true;
		// ���� ����� �� ������� �� �� ����������
		if (mShow && !mVisible) return;

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->show();
	}

	void Widget::hide()
	{
		if (false == mShow) return;
		mShow = false;
		// ���� ����� �� ������� �� �� ����������
		if (mShow && !mVisible) return;

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->hide();
	}

	void Widget::_setAlign(const IntCoord& _coord, bool _update)
	{
		// ��� ������� ��������� � �  �� ���������
		_setAlign(_coord.size(), _update);
	}

	void Widget::_setAlign(const IntSize& _size, bool _update)
	{
		if (mParent == null) return;

		bool need_move = false;
		bool need_size = false;
		IntCoord coord = mCoord;

		// �������������� ������������
		if (IS_ALIGN_RIGHT(mAlign)) {
			if (IS_ALIGN_LEFT(mAlign)) {
				// �����������
				coord.width = mCoord.width + (mParent->getWidth() - _size.width);
				need_size = true;
			} else {
				// ������� �� ������� ����
				coord.left = mCoord.left + (mParent->getWidth() - _size.width);
				need_move = true;
			}

		} else if (false == IS_ALIGN_LEFT(mAlign)) {
			// ������������ �� ����������� ��� ����������
			coord.left = (mParent->getWidth() - mCoord.width) / 2;
			need_move = true;
		}

		if (IS_ALIGN_BOTTOM(mAlign)) {
			if (IS_ALIGN_TOP(mAlign)) {
				// �����������
				coord.height = mCoord.height + (mParent->getHeight() - _size.height);
				need_size = true;
			} else {
				coord.top = mCoord.top + (mParent->getHeight() - _size.height);
				need_move = true;
			}
		} else if (false == IS_ALIGN_TOP(mAlign)) {
			// ������������ �� ��������� ��� ����������
			coord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) setPosition(coord);
			else setPosition(coord.point());
		} else if (need_size) {
			setSize(coord.size());
		} else _updateView(); // ������ ���� �� ������� ������������ � ����

	}

	void Widget::_updateView()
	{

		bool margin = mParent ? _checkMargin() : false;

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {

				// ��������
				_setVisible(false);
				// ���������� ������� ���������
				mIsMargin = margin;
				return;
			}

		} else if (false == mIsMargin) { // �� �� �������� � ���� ����������

			_setVisible(true);
			// ��� ��� ���� ����� ���������� ���� ��� ��������
			for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

			return;

		}

		// ���������� ������� ���������
		mIsMargin = margin;

		// ���� ���� ��� �����, �� �������
		_setVisible(true);

		// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateView();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

	}

	void Widget::setCaption(const Ogre::DisplayString & _caption)
	{
		if (null != mText) mText->setCaption(_caption);
	}

	const Ogre::DisplayString & Widget::getCaption()
	{
		if (null == mText) {
			static Ogre::DisplayString empty;
			return empty;
		}
		return mText->getCaption();
	}

	void Widget::setTextAlign(Align _align)
	{
		if (mText != null) mText->setTextAlign(_align);
	}

	void Widget::setColour(const Ogre::ColourValue & _colour)
	{
		if (null != mText) mText->setColour(_colour);
	}

	const Ogre::ColourValue & Widget::getColour()
	{
		return (null == mText) ? Ogre::ColourValue::ZERO : mText->getColour();
	}

	void Widget::setFontName(const Ogre::String & _font)
	{
		if (null != mText) mText->setFontName(_font);
	}

	void Widget::setFontName(const Ogre::String & _font, Ogre::ushort _height)
	{
		if (null != mText) mText->setFontName(_font, _height);
	}

	const Ogre::String & Widget::getFontName()
	{
		if (null == mText) {
			static Ogre::String empty;
			return empty;
		}
		return mText->getFontName();
	}

	void Widget::setFontHeight(Ogre::ushort _height)
	{
		if (null != mText) mText->setFontHeight(_height);
	}

	Ogre::ushort Widget::getFontHeight()
	{
		return (null == mText) ? 0 : mText->getFontHeight();
	}

	void Widget::setState(const Ogre::String & _state)
	{
		MapWidgetStateInfo::const_iterator iter = mStateInfo.find(_state);
		if (iter == mStateInfo.end()) return;
		size_t index=0;
		// ������� �������� ��������
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			CroppedRectanglePtr & info = (*skin);
			if (false == info->_isText()) info->_setUVSet(iter->second.offsets[index++]);
		}
		// ������ ���� ����� ���� ������
		if ((iter->second.colour != Ogre::ColourValue::ZERO) && (mText != null)) {
			//mText->setColour(iter->second.colour);
		}
	}

	void Widget::setEnabled(bool _enabled, bool _silent)
	{
		mEnabled = _enabled;
		if (_silent) return;

		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter) {
			(*iter)->setEnabled(_enabled);
		}

		if (mEnabled) setState("normal");
		else {
			setState("disable");
			InputManager::getInstance()._unlinkWidget(this);
		}
	}

	// ������� ������ �������� ������� ��������
	void Widget::_destroyChildWidget(WidgetPtr _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if(iter != mWidgetChild.end())
		{
			delete *iter;
			// ������� �� ������
			*iter = mWidgetChild.back();
			mWidgetChild.pop_back();
			return;
		}
		MYGUI_EXCEPT("Widget not found");
	}

	// ������� ���� �����
	void Widget::_destroyAllChildWidget()
	{
		while (false == mWidgetChild.empty()) {
			// �������� ������, ��� ��� �� ������� �������� � �������
			// � �������� � ������� ������������ �������������, �.�. ������
			WidgetPtr widget = mWidgetChild.front();
			WidgetManager::getInstance().destroyWidget(widget);
		}
	}

	const IntCoord& Widget::getClientRect()
	{
		return mCoord;
	}

	VectorWidgetPtr Widget::getChilds()
	{
		return mWidgetChild;
	}

	IntSize Widget::getTextSize()
	{
		return (null == mText) ? IntSize() : mText->getTextSize();
	}

	IntSize Widget::getTextSize(const Ogre::DisplayString& _text)
	{
		return (null == mText) ? IntSize() : mText->getTextSize(_text);
	}

	IntCoord Widget::getTextCoord()
	{
		return (null == mText) ? IntCoord() : mText->getCoord();
	}

	void Widget::setAlpha(float _alpha)
	{
		if (mAlpha == _alpha) return;
		mAlpha = _alpha;
		if (null != mParent) mRealAlpha = mAlpha * static_cast<Widget*>(mParent)->_getRealAlpha();
		else mRealAlpha = mAlpha;

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAlpha();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->setAlpha(mRealAlpha);
	}

	void Widget::_updateAlpha()
	{
		MYGUI_DEBUG_ASSERT(null != mParent, "parent must be");
		mRealAlpha = mAlpha * static_cast<Widget*>(mParent)->_getRealAlpha();
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAlpha();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->setAlpha(mRealAlpha);
	}

	LayerItem * Widget::_findLayerItem(int _left, int _top)
	{
		// ��������� ���������
		if (!mVisible || !mShow || !_checkPoint(_left, _top)) return null;
		// ���� ���� �����, ��������� ��� � �� �����
		if ((false == mMaskPeekInfo.empty()) &&
			(false == mMaskPeekInfo.peek(IntPoint(_left, _top)-mCoord.point(), mCoord))) return null;
		// ������������� ��������� ��������
		if (mEnabled) {
			// ���������� � �������
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) {
				LayerItem * item = (*widget)->_findLayerItem(_left - mCoord.left, _top - mCoord.top);
				if (item != null) return item;
			}
		}
		// ����������� ����
		return this;
	}

	void Widget::_attachToLayerKeeper(LayerKeeper * _keeper)
	{
		mLayerKeeper = _keeper;
		_attachToLayerItemKeeper(mLayerKeeper->getItem());
	}

	void Widget::_detachFromLayerKeeper()
	{
		_detachFromLayerItemKeeper();
		mLayerKeeper->leaveItem(mLayerItemKeeper);
		mLayerItemKeeper = null;
	}

	void Widget::_updateAbsolutePoint()
	{
		mAbsolutePosition = mParent->getAbsolutePosition() + mCoord.point();
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_correctView();
	}

	void Widget::setPosition(const IntPoint& _pos)
	{
		// ��������� ���������� ����������
		mAbsolutePosition += _pos - mCoord.point();
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();

		mCoord = _pos;
		_updateView();

	}

	void Widget::setPosition(const IntCoord& _coord)
	{
		// ��������� ���������� ����������
		mAbsolutePosition += _coord.point() - mCoord.point();
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();

		// ������������� ����� ���������� � ������ ������� � �������
		IntCoord old = mCoord;
		mCoord = _coord;

		bool show = true;

		// ��������� ������������
		bool margin = mParent ? _checkMargin() : false;

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {
				// ��������
				show = false;
			}
		}

		_setVisible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_setAlign(old, mIsMargin || margin);

		// ���������� ������� ���������
		mIsMargin = margin;

	}

	void Widget::setSize(const IntSize& _size)
	{
		// ������������� ����� ���������� � ������ ������� � �������
		IntSize old = mCoord.size();
		mCoord = _size;

		bool show = true;

		// ��������� ������������
		bool margin = mParent ? _checkMargin() : false;

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {
				// ��������
				show = false;
			}
		}

		_setVisible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_setAlign(old, mIsMargin || margin);

		// ���������� ������� ���������
		mIsMargin = margin;

	}

	void Widget::_attachToLayerItemKeeper(LayerItemKeeper * _item)
	{
		mLayerItemKeeper = _item;
		RenderItem * renderItem = mLayerItemKeeper->addToRenderItem(mTexture, true, false);

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			(*skin)->_createDrawItem(renderItem);
		}

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) {
			(*widget)->_attachToLayerItemKeeper(mLayerItemKeeper);
		}
	}

	void Widget::_detachFromLayerItemKeeper()
	{
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) {
			(*widget)->_detachFromLayerItemKeeper();
		}

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			(*skin)->_destroyDrawItem();
		}

		mLayerItemKeeper = null;
	}

} // namespace MyGUI
