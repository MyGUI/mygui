/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Widget.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Gui.h"
//#include "MyGUI_SubWidgetSkinInterface.h"

namespace MyGUI
{

	Widget::Widget(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		CroppedRectangleInterface(IntCoord(_coord.point(), _info->getSize()), _align, _parent), // ������ �� �����
		mText(null),
		mVisible(true),
		mEnabled(true),
		mAlpha(1),
		mColour(1.0, 1.0, 1.0, 1.0),
		mStateInfo(_info->getStateInfo()),
		mName(_name),
		mCountSharedOverlay(0)
	{
		// ��� ���������� ���������
		mWidgetEventSender = this;

		// ������������� ����������� �������� ��� �����������
		for (VectorSubWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			if (SubWidgetManager::getInstance().isSharedOverlay(*iter)) mCountSharedOverlay++;
		}

		// ��������� ��������� �������
		size_t id = 0;
		for (VectorSubWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			addSubSkin(*iter, _info->getMaterial(), id);
		}

		// ������ ��������
		const MapString & param = _info->getParams();
		if (false == param.empty()) {
			MapString::const_iterator iter = param.find("FontName");
			if (iter != param.end()) setFontName(iter->second);
			iter = param.find("FontHeight");
			if (iter != param.end()) setFontHeight(util::parseInt(iter->second));
			iter = param.find("NeedKey");
			if (iter != param.end()) setNeedKeyFocus(iter->second == "true");
			iter = param.find("AlignText");
			if (iter != param.end()) setTextAlign(SkinManager::getInstance().parseAlign(iter->second));
		}

		// ���� ����� ���� ������, ���� ���� ��������� �� ����� ���
		setState("normal");
		// � ��� ������ ���������� ������
		setSize(_coord.size());
		// ����� ����
		if ( (mParent != null) && (static_cast<WidgetPtr>(mParent)->getAlpha() != 1.0f) ) setAlpha(static_cast<WidgetPtr>(mParent)->getAlpha());
		// � ��� ��������������
		_updateView();
	}

	Widget::~Widget()
	{
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			delete (*skin);
		}
		mSubSkinChild.clear();
		_destroyAllChildWidget();

		// ������� ���� �� ������ ��������, �� �� ��� ���� ��������
		if (Gui::getInstance().removeFrameListener(this)) {
			MYGUI_ERROR("widget is not remove from frame listeners");
		}
	}

	WidgetPtr Widget::createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _coord, _align, this, _name);
		mWidgetChild.push_back(widget);
		return widget;
	}

	WidgetPtr Widget::createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _name)
	{
		Gui & gui = Gui::getInstance();
		return createWidgetT(_type, _skin, IntCoord((int)(_coord.left*gui.getViewWidth()), (int)(_coord.top*gui.getViewHeight()), (int)(_coord.width*gui.getViewWidth()), (int)(_coord.height*gui.getViewHeight())), _align, _name);
	}

	CroppedRectanglePtr  Widget::addSubSkin(const SubWidgetInfo& _info, const Ogre::String& _material, size_t & _id)
	{
		CroppedRectanglePtr sub = SubWidgetManager::getInstance().createSubWidget(_info, _material, this, _id);
		// ���� ��� ���� ������, �� ����������
		if (sub->_isText()) mText = static_cast<SubWidgetTextPtr>(sub);
		// ��������� � ����� ������
		mSubSkinChild.push_back(sub);
		return sub;
	}

	void Widget::_attachChild(CroppedRectanglePtr _basis, bool _child)
	{
		if (_child) {
			// ��� � ��� ����� ����� �����������
			MYGUI_ASSERT(mSubSkinChild.size() > 0);
			mSubSkinChild[0]->_attachChild(_basis, true);
		} else {
			// ��� �� � ���, � � ������ ����
			if (mParent != null) mParent->_attachChild(_basis, true);
		}
	}

	void Widget::_setVisible(bool _visible)
	{
		if (mVisible == _visible) return;
		mVisible = _visible;

		// ���� ������ �������������, �� �� ����������
		if (mVisible && !mShow) return;

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			if (mVisible) (*skin)->show();
			else (*skin)->hide();
		}
	}

	void Widget::show()
	{
		if (mShow) return;
		mShow = true;
		// ���� ����� �� ������� �� �� ����������
		if (mShow && !mVisible) return;

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->show();
	}

	void Widget::hide()
	{
		if (false == mShow) return;
		mShow = false;
		// ���� ����� �� ������� �� �� ����������
		if (mShow && !mVisible) return;

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->hide();
	}

	void Widget::_setAlign(int _left, int _top, int _width, int _height, bool _update)
	{
		// ��� ������� ��������� � �  �� ���������
		_setAlign(_width, _height, _update);
	}

	void Widget::_setAlign(int _width, int _height, bool _update)
	{
		if (mParent == null) return;

		bool need_move = false;
		bool need_size = false;
		int x = mCoord.left;
		int y = mCoord.top;
		int cx = mCoord.width;
		int cy = mCoord.height;

		// �������������� ������������ 
		if (mAlign & ALIGN_RIGHT) {
			if (mAlign & ALIGN_LEFT) {
				// �����������
				cx = mCoord.width + (mParent->getWidth() - _width);
				need_size = true;
			} else {
				// ������� �� ������� ����
				x = mCoord.left + (mParent->getWidth() - _width);
				need_move = true;
			}

		} else if (!(mAlign & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			x = (mParent->getWidth() - mCoord.width) / 2;
			need_move = true;
		}

		if (mAlign & ALIGN_BOTTOM) {
			if (mAlign & ALIGN_TOP) {
				// �����������
				cy = mCoord.height + (mParent->getHeight() - _height);
				need_size = true;
			} else {
				y = mCoord.top + (mParent->getHeight() - _height);
				need_move = true;
			}
		} else if (!(mAlign & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			y = (mParent->getHeight() - mCoord.height) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) setPosition(x, y, cx, cy);
			else setPosition(x, y);
		} else if (need_size) {
			setSize(cx, cy);
		} else _updateView(); // ������ ���� �� ������� ������������ � ����

	}


	void Widget::setPosition(const IntPoint& _pos)
	{
		// � ��� ������ ���������� ����� ����������
		mCoord = _pos;

		_updateView();
	}

	void Widget::setPosition(const IntCoord& _coord)
	{
		// � ��� ������ ���������� ����� ����������
		mCoord = _coord.point();

		// ������ ���������� �������
		int tmp = mCoord.width;
		mCoord.width = _coord.width;
		int width = tmp;

		tmp = mCoord.height;
		mCoord.height = _coord.height;
		int height = tmp;

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
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->_setAlign(mCoord.left, mCoord.top, width, height, mIsMargin || margin);
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->_setAlign(mCoord.left, mCoord.top, width, height, mIsMargin || margin);

		// ���������� ������� ���������
		mIsMargin = margin;

	}

	void Widget::setSize(const IntSize& _size)
	{
		// ������ ���������� �������
		int tmp = mCoord.width;
		mCoord.width = _size.width;
		int width = tmp;

		tmp = mCoord.height;
		mCoord.height = _size.height;
		int height = tmp;

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
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->_setAlign(width, height, mIsMargin || margin);
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->_setAlign(width, height, mIsMargin || margin);

		// ���������� ������� ���������
		mIsMargin = margin;

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
			for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->_correctView();

			return;

		}

		// ���������� ������� ���������
		mIsMargin = margin;

		// ���� ���� ��� �����, �� �������
		_setVisible(true);

		// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->_updateView();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->_updateView();

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

	void Widget::setAlpha(float _alpha)
	{
		if (mAlpha == _alpha) return;
		mAlpha = _alpha;
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->setAlpha(mAlpha);
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->setAlpha(mAlpha);
	}

	void Widget::setColour(const Ogre::ColourValue & _colour)
	{
		mColour = _colour;
		if (null != mText) mText->setColour(_colour);
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
		if (null == mText) return 0;
		return mText->getFontHeight();
	}

	void Widget::setState(const Ogre::String & _state)
	{
		MapWidgetStateInfo::const_iterator iter = mStateInfo.find(_state);
		if (iter == mStateInfo.end()) return;
		size_t index=0;
		// ������� �������� ��������
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			CroppedRectanglePtr & info = (*skin);
			if (false == info->_isText()) info->_setUVSet(iter->second.offsets[index++]);
		}
		// ������ ���� ����� ���� ������
		if ((iter->second.colour != Ogre::ColourValue::ZERO) && (mText != null)) {
			mText->setColour(iter->second.colour);
		}
	}

	// ���������� ��������� �� ����� � ���� ����� ��������� � ������ (������������ �� LayerItemInfo)
	LayerItemInfoPtr Widget::findItem(int _left, int _top)
	{
		// ��������� ���������
		if (!mVisible || !mShow || !_checkPoint(_left, _top)) return 0;
		// ������������� ��������� ��������
		if (!mEnabled) return this;
		// ���������� � �������
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) {
			LayerItemInfoPtr item = (*widget)->findItem(_left - mCoord.left, _top - mCoord.top);
			if (item != null) return item;
		}
		// ����������� ����
		return this;
	}

	void Widget::setEnabled(bool _enabled)
	{
		mEnabled = _enabled;
		InputManager::getInstance().unlinkWidget(this);
	}

	void Widget::attachToOverlay(Ogre::Overlay * _overlay)
	{
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			CroppedRectanglePtr & info = (*skin);
			if (false == info->_isText()) {
				Ogre::OverlayContainer * element = static_cast<Ogre::OverlayContainer*>(info->_getOverlayElement());
				if (null != element) _overlay->add2D(element);
			}
		}
	}

	void Widget::detachToOverlay(Ogre::Overlay * _overlay)
	{
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			CroppedRectanglePtr & info = (*skin);
			if (false == info->_isText()) {
				SharedPanelAlphaOverlayElement * element = static_cast<SharedPanelAlphaOverlayElement*>(info->_getOverlayElement());
				// ������� ������ ����� �������
				if (null != element) {
					_overlay->remove2D(element);
					// ���� ������� �������� ����
					element->setOverlay(0);
				}
			}
		}
	}

	// ��������������� ����� ��� ������������� ������������
	WidgetPtr Widget::parseSubWidget(const MapString & _param, const std::string & _type, const std::string & _skin, const std::string & _offset, const std::string & _align, const IntSize &_size)
	{
		// ������ ���������
		MapString::const_iterator iter = _param.find(_skin);
		if ( (iter != _param.end()) && (! iter->second.empty()) ) {
			// ��������� ����
			std::string skin = iter->second;
			FloatRect offset;
			Align align;
			// �������� � �����
			iter = _param.find(_offset);
			if (iter != _param.end()) offset = FloatRect::parse(iter->second);
			else offset.clear();
			// ������������ �����
			iter = _param.find(_align);
			if (iter != _param.end()) align = SkinManager::getInstance().parseAlign(iter->second);
			else align = ALIGN_NONE;

			offset = WidgetManager::convertOffset(offset, align, _size, mCoord.width, mCoord.height);
			return createWidgetT(_type, skin, offset.left, offset.top, offset.right, offset.bottom, align);
			
		}
		return null;
	}

	// ������� ������ �������� ������� ��������
	void Widget::_destroyChildWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < mWidgetChild.size(); index++) {
			WidgetPtr widget = mWidgetChild[index];
			if (_widget == widget) {

				delete _widget;
				_widget = null;

				// ������� �� ������
				mWidgetChild[index] = mWidgetChild[mWidgetChild.size()-1];
				mWidgetChild.pop_back();
				return;
			}
		}
		MYGUI_EXCEPT("Widget is not find");
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
	// ���������� ����������� ��������� ��� ����� ������
	size_t Widget::_getCountSharedOverlay()
	{
		return mCountSharedOverlay;
	}

	Ogre::OverlayElement* Widget::_getSharedOverlayElement()
	{
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			CroppedRectanglePtr & info = (*skin);
			if (false == info->_isText()) {
				Ogre::OverlayElement* element = info->_getSharedOverlayElement();
				if (null != element) return element;
			}
		}
		return null;
	}

	IntRect Widget::getClientRect()
	{
		return IntRect(mCoord.left, mCoord.top, mCoord.right(), mCoord.bottom());
	}

} // namespace MyGUI