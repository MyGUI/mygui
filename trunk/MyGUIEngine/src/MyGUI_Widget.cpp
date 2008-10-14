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
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_LayerKeeper.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_ISubWidgetText.h"

namespace MyGUI
{

	MYGUI_RTTI_BASE_IMPLEMENT( Widget );

	const float WIDGET_TOOLTIP_TIMEOUT = 0.5f;

	Widget::Widget(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		ICroppedRectangle(IntCoord(_coord.point(), _info->getSize()), _align, _parent), // ������ �� �����
		mOwner(static_cast<Widget*>(_parent)),
		UserData(),
		LayerItem(),
		mStateInfo(_info->getStateInfo()),
		mMaskPeekInfo(_info->getMask()),
		mText(null),
		mEnabled(true),
		mVisible(true),
		mInheritedShow(true),
		mAlpha(ALPHA_MIN),
		mName(_name),
		mTexture(_info->getTextureName()),
		mMainSkin(null),
		mIWidgetCreator(_creator),
		mInheritsAlpha(true),
		mInheritsPeek(false),
		mNeedKeyFocus(false),
		mNeedMouseFocus(true),
		mWidgetClient(null),
		mNeedToolTip(false),
		mEnableToolTip(true),
		mToolTipCurrentTime(0),
		mToolTipVisible(false),
		mToolTipOldIndex(ITEM_NONE)
	{
		// ������������ ���������� ����������
		mAbsolutePosition = _coord.point();
		if (null != mParent) mAbsolutePosition += mParent->getAbsolutePosition();

		// ��� ���������� ���������
		mWidgetEventSender = this;

		// ��������� ��������� �������
		SubWidgetManager & manager = SubWidgetManager::getInstance();
		for (VectorSubWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); ++iter) {

			ISubWidget * sub = manager.createSubWidget(*iter, this);
			mSubSkinChild.push_back(sub);

			// ���� ��������� ����������
			if (mMainSkin == null) mMainSkin = sub->castType<ISubWidgetRect>(false);
			if (mText == null) mText = sub->castType<ISubWidgetText>(false);

		}

		if (false == isRootWidget()) {
			// ���� ���� ��� ����������, �� � �� ��������� ??? ��������� ��� ��� ���������� ��� ��� ��������
			if ((null != getParent()->getLayerItemKeeper())) _attachToLayerItemKeeper(getParent()->getLayerItemKeeper());

			// ��������� ����������� ���������
			if ((!mParent->isShow()) || (!getParent()->_isInheritedShow())) {
				mInheritedShow = false;
				// �������� ������ ��� �����, ����� � ��� ��� ���
				for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->hide();
			}
		}

		setState("normal");

		// ������ ��������
		const MapString & properties = _info->getProperties();
		if (false == properties.empty()) {
			MapString::const_iterator iter = properties.end();
			if ((iter = properties.find("FontName")) != properties.end()) setFontName(iter->second);
			if ((iter = properties.find("FontHeight")) != properties.end()) setFontHeight(utility::parseInt(iter->second));
			if ((iter = properties.find("NeedKey")) != properties.end()) setNeedKeyFocus(utility::parseBool(iter->second));
			if ((iter = properties.find("NeedMouse")) != properties.end()) setNeedMouseFocus(utility::parseBool(iter->second));
			if ((iter = properties.find("AlignText")) != properties.end()) setTextAlign(Align::parse(iter->second));
			if ((iter = properties.find("Colour")) != properties.end()) setColour(utility::parseColour(iter->second));
			if ((iter = properties.find("Pointer")) != properties.end()) mPointer = iter->second;
			if ((iter = properties.find("Show")) != properties.end()) { utility::parseBool(iter->second) ? show() : hide(); }
		}

		// ���������� �����, ������������� �� ���� �������������
		setAlpha(ALPHA_MAX);

		// ������� �����
		const VectorChildSkinInfo& child = _info->getChild();
		for (VectorChildSkinInfo::const_iterator iter=child.begin(); iter!=child.end(); ++iter) {
			WidgetPtr widget = createWidgetT(iter->type, iter->skin, iter->coord, iter->align);
			widget->_setInternalData(iter->name);
			// ��������� UserString ����������
			for (MapString::const_iterator prop=iter->params.begin(); prop!=iter->params.end(); ++prop) {
				widget->setUserString(prop->first, prop->second);
			}
		}

		// � ��� ������ ���������� ������
		setSize(_coord.size());

	}

	Widget::~Widget()
	{
		Gui::getInstance().eventFrameStart -= newDelegate(this, &Widget::frameEntered);
		//Gui::getInstance().removeFrameListener(newDelegate(this, &Widget::frameEntered));

		if (mToolTipVisible) eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));

		_detachFromLayerItemKeeper();

		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			delete (*skin);
		}

		mSubSkinChild.clear();
		_destroyAllChildWidget();
	}

	WidgetPtr Widget::_createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _coord, _align, this, this, _name);
		mWidgetChild.push_back(widget);
		return widget;
	}

	WidgetPtr Widget::createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _name)
	{
		return createWidgetT(_type, _skin, WidgetManager::getInstance().convertRelativeToInt(_coord, this), _align, _name);
	}

	void Widget::_setAlign(const IntCoord& _coord, bool _update)
	{
		// ��� ������� ��������� � �  �� ���������
		_setAlign(_coord.size(), _update);
	}

	void Widget::_setAlign(const IntSize& _size, bool _update)
	{
		if (null == mParent) return;

		bool need_move = false;
		bool need_size = false;
		IntCoord coord = mCoord;

		// �������������� ������������
		if (mAlign.isHStretch()) {
			// �����������
			coord.width = mCoord.width + (mParent->getWidth() - _size.width);
			need_size = true;
		}
		else if (mAlign.isRight()) {
			// ������� �� ������� ����
			coord.left = mCoord.left + (mParent->getWidth() - _size.width);
			need_move = true;
		}
		else if (mAlign.isHCenter()) {
			// ������������ �� ����������� ��� ����������
			coord.left = (mParent->getWidth() - mCoord.width) / 2;
			need_move = true;
		}

		if (mAlign.isVStretch()) {
			// �����������
			coord.height = mCoord.height + (mParent->getHeight() - _size.height);
			need_size = true;
		}
		else if (mAlign.isBottom()) {
			// ������� �� ������� ����
			coord.top = mCoord.top + (mParent->getHeight() - _size.height);
			need_move = true;
		}
		else if (mAlign.isVCenter()) {
			// ������������ �� ��������� ��� ����������
			coord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) setPosition(coord);
			else setPosition(coord.point());
		}
		else if (need_size) {
			setSize(coord.size());
		}
		else _updateView(); // ������ ���� �� ������� ������������ � ����

	}

	void Widget::_updateView()
	{

		bool margin = mParent ? _checkMargin() : false;

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {

				// ���������� ������� ���������
				mIsMargin = margin;

				// ��������
				_setVisible(false);

				// ��� ��� ���� ����� ���������� ���� ��� ��������
				//for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

				// ��� �������� ������ ���� ����������
				for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateView();

				return;
			}

		} else if (false == mIsMargin) { // �� �� �������� � ���� ����������

			// ���������� ������� ���������
			//mIsMargin = margin;

			//_setVisible(true);
			// ��� ��� ���� ����� ���������� ���� ��� ��������
			for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

			return;

		}

		// ���������� ������� ���������
		mIsMargin = margin;

		// ���� ���� ��� �����, �� �������
		_setVisible(true);

		// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateView();
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

	}

	void Widget::setCaption(const Ogre::UTFString & _caption)
	{
		if (null != mText) mText->setCaption(_caption);
	}

	const Ogre::UTFString & Widget::getCaption()
	{
		if (null == mText) {
			static Ogre::UTFString empty;
			return empty;
		}
		return mText->getCaption();
	}

	void Widget::setTextAlign(Align _align)
	{
		if (mText != null) mText->setTextAlign(_align);
	}

	Align Widget::getTextAlign()
	{
		if (mText != null) return mText->getTextAlign();
		return Align::Default;
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

	const std::string & Widget::getFontName()
	{
		if (null == mText) {
			static std::string empty;
			return empty;
		}
		return mText->getFontName();
	}

	void Widget::setFontHeight(uint16 _height)
	{
		if (null != mText) mText->setFontHeight(_height);
	}

	uint16 Widget::getFontHeight()
	{
		return (null == mText) ? 0 : mText->getFontHeight();
	}

	void Widget::setState(const Ogre::String & _state)
	{
		MapWidgetStateInfo::const_iterator iter = mStateInfo.find(_state);
		if (iter == mStateInfo.end()) return;
		size_t index=0;
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin, ++index) {
			ISubWidget * info = (*skin);
			void * data = (*iter).second[index];
			if (data != null) info->_setStateData(data);
		}
	}

	void Widget::setEnabled(bool _enabled)
	{
		mEnabled = _enabled;

		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter) {
			(*iter)->setEnabled(_enabled);
		}

		if (mEnabled) setState("normal");
		else {
			setState("disable");
			InputManager::getInstance()._unlinkWidget(this);
		}
	}

	// ������� ����������
	void Widget::_destroyChildWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(null != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end()) {

			// ��������� ���������
			MyGUI::WidgetPtr widget = *iter;

			// ������� �� ������
			*iter = mWidgetChild.back();
			mWidgetChild.pop_back();

			// ���������� �� ����
			WidgetManager::getInstance().unlinkFromUnlinkers(_widget);

			// ���������������� ��������
			_deleteWidget(widget);
		}
		else MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
	}

	// ������� ���� �����
	void Widget::_destroyAllChildWidget()
	{
		WidgetManager & manager = WidgetManager::getInstance();
		while (false == mWidgetChild.empty()) {

			// ����� ���� ���������, ����� ��������� �������� ������� ���
			WidgetPtr widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// ���������� �� ����
			manager.unlinkFromUnlinkers(widget);

			// � ���� ������, ��� ��� ��� ������ � ������ ���
			delete widget;
		}
	}

	IntCoord Widget::getClientCoord()
	{
		if (mWidgetClient != null) return mWidgetClient->getCoord();
		return IntCoord(0, 0, mCoord.width, mCoord.height);
	}

	IntSize Widget::getTextSize()
	{
		return (null == mText) ? IntSize() : mText->getTextSize();
	}

	IntCoord Widget::getTextCoord()
	{
		return (null == mText) ? IntCoord() : mText->getCoord();
	}

	void Widget::setAlpha(float _alpha)
	{
		if (mAlpha == _alpha) return;
		mAlpha = _alpha;
		if (null != mParent) mRealAlpha = mAlpha * (mInheritsAlpha ? static_cast<Widget*>(mParent)->_getRealAlpha() : ALPHA_MAX);
		else mRealAlpha = mAlpha;

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAlpha();
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->setAlpha(mRealAlpha);
	}

	void Widget::_updateAlpha()
	{
		MYGUI_DEBUG_ASSERT(null != mParent, "Widget must have parent");
		mRealAlpha = mAlpha * (mInheritsAlpha ? static_cast<Widget*>(mParent)->_getRealAlpha() : ALPHA_MAX);
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAlpha();
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->setAlpha(mRealAlpha);
	}

	LayerItem * Widget::_findLayerItem(int _left, int _top)
	{
		// ��������� ���������
		if (!mVisible
			|| !mEnabled
			|| !mShow
			|| (!mNeedMouseFocus && !mInheritsPeek)
			|| !_checkPoint(_left, _top)
			// ���� ���� �����, ��������� ��� � �� �����
			|| ((!mMaskPeekInfo->empty()) && (!mMaskPeekInfo->peek(IntPoint(_left - mCoord.left, _top - mCoord.top), mCoord))))
				return null;
		// ���������� � �������
		for (VectorWidgetPtr::reverse_iterator widget= mWidgetChild.rbegin(); widget != mWidgetChild.rend(); ++widget) {
			LayerItem * item = (*widget)->_findLayerItem(_left - mCoord.left, _top - mCoord.top);
			if (item != null) return item;
		}
		// ����������� ����
		return mInheritsPeek ? null : this;
	}

	void Widget::_updateAbsolutePoint()
	{
		mAbsolutePosition = mParent->getAbsolutePosition() + mCoord.point();
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_correctView();
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
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_setAlign(old, mIsMargin || margin);

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
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_setAlign(old, mIsMargin || margin);

		// ���������� ������� ���������
		mIsMargin = margin;

	}

	void Widget::_attachToLayerItemKeeper(LayerItemKeeper * _item)
	{
		MYGUI_DEBUG_ASSERT(null != _item, "attached item must be valid");

		// ���������, ����� ����������� ���� ����� �������������
		setLayerItemKeeper(_item);

		RenderItem * renderItem = null;

		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			// ������� ������ ���� ���� ���� ���� �� ��������� �������
			if ((null == renderItem) && (*skin)->firstQueue()) {
				renderItem = _item->addToRenderItem(mTexture, true, false);
			}
			(*skin)->_createDrawItem(_item, renderItem);
		}

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) {
			(*widget)->_attachToLayerItemKeeper(_item);
		}
	}

	void Widget::_detachFromLayerItemKeeper()
	{
		// �� ��� ����������
		if (null == getLayerItemKeeper()) return;

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) {
			(*widget)->_detachFromLayerItemKeeper();
		}

		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			(*skin)->_destroyDrawItem();
		}

		// ������� 
		setLayerItemKeeper(null);
	}

	void Widget::_setUVSet(const FloatRect& _rect)
	{
		if (null != mMainSkin) mMainSkin->_setUVSet(_rect);
	}

	void Widget::_setTextureName(const Ogre::String& _texture)
	{
		if (_texture == mTexture) return;
		mTexture = _texture;

		// ���� �� ������������, �� ������� ����, ������ ��������, � ����� �������
		LayerItemKeeper * save = getLayerItemKeeper();
		if (null != save) {
			// ����� ������� ����� ��� ������
			_detachFromLayerItemKeeper();
			mTexture = _texture;
			_attachToLayerItemKeeper(save);
		}

	}

	const Ogre::String& Widget::_getTextureName()
	{
		return mTexture;
	}

	void Widget::_setVisible(bool _visible)
	{
		if (mVisible == _visible) return;
		mVisible = _visible;

		// ������ ���������
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			(*skin)->_updateView();
		}
	}

	void Widget::show()
	{
		if (mShow) return;
		mShow = true;

		if (mInheritedShow) {
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_inheritedShow();
			for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->show();
		}

	}

	void Widget::hide()
	{
		if (false == mShow) return;
		mShow = false;

		// ���� �� ��� ������ �����, �� ��������� �� �����
		if (mInheritedShow) {
			for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->hide();
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_inheritedHide();
		}

	}

	void Widget::_inheritedShow()
	{
		if (mInheritedShow) return;
		mInheritedShow = true;

		if (mShow) {
			for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->show();
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_inheritedShow();
		}

	}

	void Widget::_inheritedHide()
	{
		if (false == mInheritedShow) return;
		mInheritedShow = false;

		if (mShow) {
			for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->hide();
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_inheritedHide();
		}

	}

	// ���� ��������� ������� ��� ��������
	void Widget::_forcePeek(WidgetPtr _widget)
	{
		size_t size = mWidgetChild.size();
		if ( (size < 2) || (mWidgetChild[size-1] == _widget) ) return;
		for (size_t pos=0; pos<size; pos++) {
			if (mWidgetChild[pos] == _widget) {
				mWidgetChild[pos] = mWidgetChild[size-1];
				mWidgetChild[size-1] = _widget;
				return;
			}
		}
	}

	const std::string& Widget::getLayerName()
	{
		LayerKeeper * keeper = getLayerKeeper();
		if (null == keeper) {
			static std::string empty;
			return empty;
		}
		return keeper->getName();
	}

	void Widget::getContainer(WidgetPtr & _list, size_t & _index)
	{
		_list = null;
		_index = ITEM_NONE;
		requestGetContainer(this, _list, _index);
	}

	WidgetPtr Widget::findWidget(const std::string & _name)
	{
		if (_name == mName) return this;
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) {
			WidgetPtr find = (*widget)->findWidget(_name);
			if (null != find) return find;
		}
		return null;
	}

	void Widget::setNeedToolTip(bool _need)
	{
		if (mNeedToolTip == _need) return;
		mNeedToolTip = _need;

		if (mNeedToolTip) {
			Gui::getInstance().eventFrameStart += newDelegate(this, &Widget::frameEntered);
			mToolTipCurrentTime = 0;
		}
		else {
			Gui::getInstance().eventFrameStart -= newDelegate(this, &Widget::frameEntered);
		}
	}

	void Widget::frameEntered(float _frame)
	{
		if ( ! mEnableToolTip) return;

		IntPoint point = InputManager::getInstance().getMousePosition();

		if (mToolTipOldPoint != point) {

			mToolTipCurrentTime = 0;

			bool inside = getAbsoluteRect().inside(point);
			if (inside) {
				inside = false;
				// ��������� �� ����������� �� ���
				WidgetPtr widget = InputManager::getInstance().getMouseFocusWidget();
				while (widget != 0) {
					if (this->compare(widget)) {
						inside = true;
						break;
					}
					widget = widget->getParent();
				}

				if (inside) {
					// ������ �������, �� ��������� �� ������ ������ ����
					size_t index = getContainerIndex(point);
					if (mToolTipOldIndex != index) {
						if (mToolTipVisible) {
							mToolTipCurrentTime = 0;
							mToolTipVisible = false;
							eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
						}
						mToolTipOldIndex = index;
					}

				}
				else {
					if (mToolTipVisible) {
						mToolTipCurrentTime = 0;
						mToolTipVisible = false;
						eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
					}
				}

			}
			else {
				if (mToolTipVisible) {
					mToolTipCurrentTime = 0;
					mToolTipVisible = false;
					eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
				}
			}

			mToolTipOldPoint = point;
		}
		else {

			bool inside = getAbsoluteRect().inside(point);
			if (inside) {
				inside = false;
				// ��������� �� ����������� �� ���
				WidgetPtr widget = InputManager::getInstance().getMouseFocusWidget();
				while (widget != 0) {
					if (widget->getName() == mName) {
						inside = true;
						break;
					}
					widget = widget->getParent();
				}

				if (inside) {
					if ( ! mToolTipVisible) {
						mToolTipCurrentTime += _frame;
						if (mToolTipCurrentTime > WIDGET_TOOLTIP_TIMEOUT) {
							mToolTipVisible = true;
							eventToolTip(this, ToolTipInfo(ToolTipInfo::Show, mToolTipOldIndex, point));
						}
					}
				}
			}
		}
	}

	void Widget::enableToolTip(bool _enable)
	{
		if (_enable == mEnableToolTip) return;
		mEnableToolTip = _enable;

		if ( ! mEnableToolTip) {
			if (mToolTipVisible) {
				mToolTipCurrentTime = 0;
				mToolTipVisible = false;
				eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
			}
		}
		else {
			mToolTipCurrentTime = 0;
		}
	}

	void Widget::resetContainer(bool _updateOnly)
	{
		if ( mEnableToolTip) {
			if (mToolTipVisible) {
				mToolTipVisible = false;
				eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
			}
			mToolTipCurrentTime = 0;
			mToolTipOldIndex = ITEM_NONE;
		}
	}

	EnumeratorWidgetPtr Widget::getEnumerator()
	{
		return Enumerator<VectorWidgetPtr>(mWidgetChild.begin(), mWidgetChild.end());
	}

	void Widget::setMaskPeek(const std::string & _filename)
	{
		if (mOwnMaskPeekInfo.load(_filename)) {
			mMaskPeekInfo = &mOwnMaskPeekInfo;
		}
		else {
			MYGUI_LOG(Error, "mask not load '" << _filename << "'");
		}
		//SkinManager::getInstance().
	}

} // namespace MyGUI
