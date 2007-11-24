
#include "Widget.h"
#include "SkinManager.h"
#include "WidgetManager.h"

namespace MyGUI
{

	Widget::Widget(int _x, int _y, int _cx, int _cy, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		BasisWidget(_x, _y, _info->getSize().width, _info->getSize().height, _align, _parent), // ������ �� �����
		m_text(0),
		m_visible(true),
		m_enable(true),
		m_alpha(1.0),
		m_color(1.0, 1.0, 1.0, 1.0),
		m_stateInfo(_info->getStateInfo()),
		m_name(_name)
	{
		// ��� ���������� ���������
		m_widgetEventSender = this;

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
		size(_cx, _cy);
		// ����� ����
		if ( (mParent != null) && (static_cast<WidgetPtr>(mParent)->getAlpha() != 1.0f) ) setAlpha(static_cast<WidgetPtr>(mParent)->getAlpha());
		// � ��� ��������������
		update();
	}

	Widget::~Widget()
	{

		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			delete (*skin);
		}
		m_subSkinChild.clear();
		destroyWidget();

	}


	WidgetPtr Widget::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, Align _align, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _x, _y, _cx, _cy, _align, this, _name);
		mWidgetChild.push_back(widget);
		return widget;
	}

	WidgetPtr Widget::createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, Align _align, const Ogre::String & _name)
	{
		Gui & gui = Gui::getInstance();
		return createWidget(_type, _skin, (int)(_x*gui.getWidth()), (int)(_y*gui.getHeight()), (int)(_cx*gui.getWidth()), (int)(_cy*gui.getHeight()), _align, _name);
	}

	BasisWidgetPtr  Widget::addSubSkin(const BasisWidgetInfo& _info, const Ogre::String& _material)
	{
		BasisWidgetPtr sub = BasisWidgetManager::getInstance().createBasisWidget(_info, _material, this);
		// ���� ��� ���� ������, �� ����������
		if (sub->isText()) m_text = sub;
		// ��������� � ����� ������
		m_subSkinChild.push_back(sub);
		return sub;
	}

	void Widget::attach(BasisWidgetPtr _basis, bool _child)
	{
		if (_child) {
			// ��� � ��� ����� ����� �����������
			assert(m_subSkinChild.size() > 0);
			m_subSkinChild[0]->attach(_basis, true);
		} else {
			// ��� �� � ���, � � ������ ����
			if (mParent != null) mParent->attach(_basis, true);
		}
	}

	void Widget::visible(bool _visible)
	{

		if (m_visible == _visible) return;
		m_visible = _visible;

		// ���� ������ �������������, �� �� ����������
		if (_visible && !mShow) return;

		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(m_visible);
	}

	void Widget::show(bool _show)
	{
		if (mShow == _show) return;
		mShow = _show;
		// ���� ����� �� ������� �� �� ����������
		if (_show && !m_visible) return;

		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(mShow);
	}

	void Widget::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		// ��� ������� ��������� � �  �� ���������
		align(_cx, _cy, _update);
	}

	void Widget::align(int _cx, int _cy, bool _update)
	{
		if (mParent == null) return;

		bool need_move = false;
		bool need_size = false;
		int x = m_x;
		int y = m_y;
		int cx = m_cx;
		int cy = m_cy;

		// �������������� ������������ 
		if (mAlign & ALIGN_RIGHT) {
			if (mAlign & ALIGN_LEFT) {
				// �����������
				cx = m_cx + (mParent->width() - _cx);
				need_size = true;
			} else {
				// ������� �� ������� ����
				x = m_x + (mParent->width() - _cx);
				need_move = true;
			}

		} else if (!(mAlign & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			x = (mParent->width() - m_cx) / 2;
			need_move = true;
		}

		if (mAlign & ALIGN_BOTTOM) {
			if (mAlign & ALIGN_TOP) {
				// �����������
				cy = m_cy + (mParent->height() - _cy);
				need_size = true;
			} else {
				y = m_y + (mParent->height() - _cy);
				need_move = true;
			}
		} else if (!(mAlign & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			y = (mParent->height() - m_cy) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) move(x, y, cx, cy);
			else move(x, y);
		} else if (need_size) {
			size(cx, cy);
		} else update(); // ������ ���� �� ������� ������������ � ����

	}


	void Widget::move(int _x, int _y)
	{
		// � ��� ������ ���������� ����� ����������
		m_x = _x;
		m_y = _y;

		update();
	}

	void Widget::move(int _x, int _y, int _cx, int _cy)
	{
		// � ��� ������ ���������� ����� ����������
		m_x = _x;
		m_y = _y;

		// ������ ���������� �������
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		bool show = true;

		// ��������� ������������
		bool margin = mParent ? check_margin() : false;

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (check_outside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->align(m_x, m_y, _cx, _cy, mMargin || margin);
		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(m_x, m_y, _cx, _cy, mMargin || margin);

		// ���������� ������� ���������
		mMargin = margin;

	}

	void Widget::size(int _cx, int _cy)
	{
		// ������ ���������� �������
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		bool show = true;

		// ��������� ������������
		bool margin = mParent ? check_margin() : false;

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (check_outside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->align(_cx, _cy, mMargin || margin);
		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(_cx, _cy, mMargin || margin);

		// ���������� ������� ���������
		mMargin = margin;

	}

	void Widget::update()
	{

		bool margin = mParent ? check_margin() : false;

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (check_outside()) {

				// ��������
				visible(false);
				// ���������� ������� ���������
				mMargin = margin;
				return;
			}

		} else if (!mMargin) { // �� �� �������� � ���� ����������

			visible(true);
			// ��� ��� ���� ����� ���������� ���� ��� ��������
			for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->correct();

			return;

		}

		// ���������� ������� ���������
		mMargin = margin;

		// ���� ���� ��� �����, �� �������
		visible(true);

		// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->update();
		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->update();

	}

	void Widget::setCaption(const Ogre::DisplayString & _caption)
	{
		if (m_text == null) return;
		m_text->setCaption(_caption);
	}

	const Ogre::DisplayString & Widget::getCaption()
	{
		if (m_text == null) return BasisWidget::getCaption();
		return m_text->getCaption();
	}

	void Widget::setTextAlign(Align _align)
	{
		if (m_text != null) m_text->setTextAlign(_align);
	}

	void Widget::setAlpha(float _alpha)
	{
		if (m_alpha == _alpha) return;
		m_alpha = _alpha;
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->setAlpha(m_alpha);
		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->setAlpha(m_alpha);
	}

	void Widget::setColour(const Ogre::ColourValue & _color)
	{
		m_color = _color;
		if (!m_text) return;
		m_text->setColour(_color);
	}

	void Widget::setFontName(const Ogre::String & _font)
	{
		if (!m_text) return;
		m_text->setFontName(_font);
	}

	void Widget::setFontName(const Ogre::String & _font, Ogre::ushort _height)
	{
		if (!m_text) return;
		m_text->setFontName(_font, _height);
	}

	const Ogre::String & Widget::getFontName()
	{
		if (!m_text) return BasisWidget::getFontName();
		return m_text->getFontName();
	}

	void Widget::setFontHeight(Ogre::ushort _height)
	{
		if (!m_text) return;
		m_text->setFontHeight(_height);
	}

	Ogre::ushort Widget::getFontHeight()
	{
		if (!m_text) return 0;
		return m_text->getFontHeight();
	}

	void Widget::setState(const Ogre::String & _state)
	{
		MapWidgetStateInfo::const_iterator iter = m_stateInfo.find(_state);
		if (iter == m_stateInfo.end()) return;
		size_t index=0;
		// ������� �������� ��������
		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			(*skin)->setUVSet(iter->second.offsets[index++]);
		}
		// ������ ���� ����� ���� ������
		if ((iter->second.color != Ogre::ColourValue::ZERO) && (m_text != null)) {
			m_text->setColour(iter->second.color);
		}
	}

	// ���������� ��������� �� ����� � ���� ����� ��������� � ������ (������������ �� LayerItemInfo)
	LayerItemInfoPtr Widget::findItem(int _x, int _y)
	{
		// ��������� ���������
		if (!m_visible || !mShow || !check_point(_x, _y)) return 0;
		// ������������� ��������� ��������
		if (!m_enable) return this;
		// ���������� � �������
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) {
			LayerItemInfoPtr item = (*widget)->findItem(_x - m_x, _y - m_y);
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
		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			if (!(*skin)->isText()) _overlay->add2D(static_cast<Ogre::OverlayContainer*>((*skin)->getOverlayElement()));
		}
	}

	void Widget::detachToOverlay(Ogre::Overlay * _overlay)
	{
		for (VectorBasisWidgetPtr::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
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

			offset = WidgetManager::convertOffset(offset, align, _size, m_cx, m_cy);
			return createWidget(_type, skin, offset.left, offset.top, offset.right, offset.bottom, align);
			
		}
		return null;
	}

} // namespace MyGUI