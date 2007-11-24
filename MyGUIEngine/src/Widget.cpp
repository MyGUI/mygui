
#include "Widget.h"
#include "SkinManager.h"
#include "WidgetManager.h"

namespace MyGUI
{

	Widget::Widget(int _left, int _top, int _width, int _height, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		BasisWidget(_left, _top, _info->getSize().width, _info->getSize().height, _align, _parent), // ������ �� �����
		mText(0),
		mVisible(true),
		mEnable(true),
		mAlpha(1.0),
		mColour(1.0, 1.0, 1.0, 1.0),
		mStateInfo(_info->getStateInfo()),
		mName(_name)
	{
		// ��� ���������� ���������
		mWidgetEventSender = this;

		// ��������� ��������� �������
		for (VectorBasisWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			addSubSkin(*iter, _info->getMaterial());
		}

		// ������ ��������
		const MapString & param = _info->getParams();
		if (!param.empty()) {
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
		size(_width, _height);
		// ����� ����
		if ( (mParent != null) && (static_cast<WidgetPtr>(mParent)->getAlpha() != 1.0f) ) setAlpha(static_cast<WidgetPtr>(mParent)->getAlpha());
		// � ��� ��������������
		update();
	}

	Widget::~Widget()
	{

		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			delete (*skin);
		}
		mSubSkinChild.clear();
		destroyWidget();

	}


	WidgetPtr Widget::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _left, _top, _width, _height, _align, this, _name);
		mWidgetChild.push_back(widget);
		return widget;
	}

	WidgetPtr Widget::createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _name)
	{
		Gui & gui = Gui::getInstance();
		return createWidget(_type, _skin, (int)(_left*gui.getWidth()), (int)(_top*gui.getHeight()), (int)(_width*gui.getWidth()), (int)(_height*gui.getHeight()), _align, _name);
	}

	BasisWidgetPtr  Widget::addSubSkin(const BasisWidgetInfo& _info, const Ogre::String& _material)
	{
		BasisWidgetPtr sub = BasisWidgetManager::getInstance().createBasisWidget(_info, _material, this);
		// ���� ��� ���� ������, �� ����������
		if (sub->isText()) mText = sub;
		// ��������� � ����� ������
		mSubSkinChild.push_back(sub);
		return sub;
	}

	void Widget::attach(BasisWidgetPtr _basis, bool _child)
	{
		if (_child) {
			// ��� � ��� ����� ����� �����������
			assert(mSubSkinChild.size() > 0);
			mSubSkinChild[0]->attach(_basis, true);
		} else {
			// ��� �� � ���, � � ������ ����
			if (mParent != null) mParent->attach(_basis, true);
		}
	}

	void Widget::visible(bool _visible)
	{

		if (mVisible == _visible) return;
		mVisible = _visible;

		// ���� ������ �������������, �� �� ����������
		if (_visible && !mShow) return;

		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->show(mVisible);
	}

	void Widget::show(bool _show)
	{
		if (mShow == _show) return;
		mShow = _show;
		// ���� ����� �� ������� �� �� ����������
		if (_show && !mVisible) return;

		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->show(mShow);
	}

	void Widget::align(int _left, int _top, int _width, int _height, bool _update)
	{
		// ��� ������� ��������� � �  �� ���������
		align(_width, _height, _update);
	}

	void Widget::align(int _width, int _height, bool _update)
	{
		if (mParent == null) return;

		bool need_move = false;
		bool need_size = false;
		int x = mLeft;
		int y = mTop;
		int cx = mWidth;
		int cy = mHeight;

		// �������������� ������������ 
		if (mAlign & ALIGN_RIGHT) {
			if (mAlign & ALIGN_LEFT) {
				// �����������
				cx = mWidth + (mParent->getWidth() - _width);
				need_size = true;
			} else {
				// ������� �� ������� ����
				x = mLeft + (mParent->getWidth() - _width);
				need_move = true;
			}

		} else if (!(mAlign & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			x = (mParent->getWidth() - mWidth) / 2;
			need_move = true;
		}

		if (mAlign & ALIGN_BOTTOM) {
			if (mAlign & ALIGN_TOP) {
				// �����������
				cy = mHeight + (mParent->getHeight() - _height);
				need_size = true;
			} else {
				y = mTop + (mParent->getHeight() - _height);
				need_move = true;
			}
		} else if (!(mAlign & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			y = (mParent->getHeight() - mHeight) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) move(x, y, cx, cy);
			else move(x, y);
		} else if (need_size) {
			size(cx, cy);
		} else update(); // ������ ���� �� ������� ������������ � ����

	}


	void Widget::move(int _left, int _top)
	{
		// � ��� ������ ���������� ����� ����������
		mLeft = _left;
		mTop = _top;

		update();
	}

	void Widget::move(int _left, int _top, int _width, int _height)
	{
		// � ��� ������ ���������� ����� ����������
		mLeft = _left;
		mTop = _top;

		// ������ ���������� �������
		int tmp = mWidth;
		mWidth = _width;
		_width = tmp;

		tmp = mHeight;
		mHeight = _height;
		_height = tmp;

		bool show = true;

		// ��������� ������������
		bool margin = mParent ? checkMargin() : false;

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (checkOutside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->align(mLeft, mTop, _width, _height, mMargin || margin);
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->align(mLeft, mTop, _width, _height, mMargin || margin);

		// ���������� ������� ���������
		mMargin = margin;

	}

	void Widget::size(int _width, int _height)
	{
		// ������ ���������� �������
		int tmp = mWidth;
		mWidth = _width;
		_width = tmp;

		tmp = mHeight;
		mHeight = _height;
		_height = tmp;

		bool show = true;

		// ��������� ������������
		bool margin = mParent ? checkMargin() : false;

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (checkOutside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->align(_width, _height, mMargin || margin);
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->align(_width, _height, mMargin || margin);

		// ���������� ������� ���������
		mMargin = margin;

	}

	void Widget::update()
	{

		bool margin = mParent ? checkMargin() : false;

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (checkOutside()) {

				// ��������
				visible(false);
				// ���������� ������� ���������
				mMargin = margin;
				return;
			}

		} else if (!mMargin) { // �� �� �������� � ���� ����������

			visible(true);
			// ��� ��� ���� ����� ���������� ���� ��� ��������
			for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->correct();

			return;

		}

		// ���������� ������� ���������
		mMargin = margin;

		// ���� ���� ��� �����, �� �������
		visible(true);

		// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->update();
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->update();

	}

	void Widget::setCaption(const Ogre::DisplayString & _caption)
	{
		if (mText == null) return;
		mText->setCaption(_caption);
	}

	const Ogre::DisplayString & Widget::getCaption()
	{
		if (mText == null) return BasisWidget::getCaption();
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
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->setAlpha(mAlpha);
	}

	void Widget::setColour(const Ogre::ColourValue & _colour)
	{
		mColour = _colour;
		if (!mText) return;
		mText->setColour(_colour);
	}

	void Widget::setFontName(const Ogre::String & _font)
	{
		if (!mText) return;
		mText->setFontName(_font);
	}

	void Widget::setFontName(const Ogre::String & _font, Ogre::ushort _height)
	{
		if (!mText) return;
		mText->setFontName(_font, _height);
	}

	const Ogre::String & Widget::getFontName()
	{
		if (!mText) return BasisWidget::getFontName();
		return mText->getFontName();
	}

	void Widget::setFontHeight(Ogre::ushort _height)
	{
		if (!mText) return;
		mText->setFontHeight(_height);
	}

	Ogre::ushort Widget::getFontHeight()
	{
		if (!mText) return 0;
		return mText->getFontHeight();
	}

	void Widget::setState(const Ogre::String & _state)
	{
		MapWidgetStateInfo::const_iterator iter = mStateInfo.find(_state);
		if (iter == mStateInfo.end()) return;
		size_t index=0;
		// ������� �������� ��������
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			(*skin)->setUVSet(iter->second.offsets[index++]);
		}
		// ������ ���� ����� ���� ������
		if ((iter->second.color != Ogre::ColourValue::ZERO) && (mText != null)) {
			mText->setColour(iter->second.color);
		}
	}

	// ���������� ��������� �� ����� � ���� ����� ��������� � ������ (������������ �� LayerItemInfo)
	LayerItemInfoPtr Widget::findItem(int _left, int _top)
	{
		// ��������� ���������
		if (!mVisible || !mShow || !checkPoint(_left, _top)) return 0;
		// ������������� ��������� ��������
		if (!mEnable) return this;
		// ���������� � �������
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) {
			LayerItemInfoPtr item = (*widget)->findItem(_left - mLeft, _top - mTop);
			if (item != null) return item;
		}
		// ����������� ����
		return this;
	}

	// ������� ������ �������� ������� ��������
	void Widget::destroyWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < mWidgetChild.size(); index++) {
			WidgetPtr widget = mWidgetChild[index];
			if (_widget == widget) {
				// ������� ���� ���
				WidgetManager::getInstance().clearName(_widget);
				delete _widget;
				_widget = 0;

				// ������� �� ������
				mWidgetChild[index] = mWidgetChild[mWidgetChild.size()-1];
				mWidgetChild.pop_back();
				return;
			}
		}
	}

	// ������� ���� �����
	void Widget::destroyWidget()
	{
		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); iter++) {
			WidgetPtr widget = *iter;
			// ������� ���� ���
			WidgetManager::getInstance().clearName(widget);
			// � �������
			delete widget;
		}
		mWidgetChild.clear();
	}

	void Widget::attachToOverlay(Ogre::Overlay * _overlay)
	{
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			if (!(*skin)->isText()) _overlay->add2D(static_cast<Ogre::OverlayContainer*>((*skin)->getOverlayElement()));
		}
	}

	void Widget::detachToOverlay(Ogre::Overlay * _overlay)
	{
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			if (!(*skin)->isText()) {
				PanelAlphaOverlayElement * element = static_cast<PanelAlphaOverlayElement*>((*skin)->getOverlayElement());
				_overlay->remove2D(element);
				// ���� ������� �������� ����
				element->setOverlay(0);
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
			if (iter != _param.end()) offset = util::parseFloatRect(iter->second);
			else offset.clear();
			// ������������ �����
			iter = _param.find(_align);
			if (iter != _param.end()) align = SkinManager::getInstance().parseAlign(iter->second);
			else align = ALIGN_NONE;

			offset = WidgetManager::convertOffset(offset, align, _size, mWidth, mHeight);
			return createWidget(_type, skin, offset.left, offset.top, offset.right, offset.bottom, align);
			
		}
		return null;
	}

} // namespace MyGUI