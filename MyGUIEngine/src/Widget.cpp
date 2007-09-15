
#include "Widget.h"
#include "BasisWidgetManager.h"
#include "WidgetManager.h"
#include "WidgetParser.h"
#include "Gui.h"

namespace MyGUI
{
	// ������� ������� ��� ����� �������
	namespace factory { WidgetFactory<Widget> WidgetFactoryInstance; }
	// ������ ������ ��� Widget
	namespace parser { WidgetParser WidgetParserInstance; }

	Widget::Widget(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
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
		for (BasisInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			addSubSkin(*iter, _info->getMaterial());
		}

		// ������ ��������
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
			SkinParam::const_iterator iter = param.find("FontName");
			if (iter != param.end()) setFontName(iter->second);
			iter = param.find("FontHeight");
			if (iter != param.end()) setFontHeight(parseInt(iter->second));
			iter = param.find("NeedKey");
			if (iter != param.end()) setNeedKeyFocus(iter->second == "true");
			iter = param.find("AlignText");
			if (iter != param.end()) setTextAlign(SkinManager::getInstance().parseAlign(iter->second));
		}

		// ���� ����� ���� ������, ���� ���� ��������� �� ����� ���
		setState("normal");
		// � ��� ������ ���������� ������
		size(_cx, _cy);
		update();
	}

	Widget::~Widget()
	{

		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			delete (*skin);
		}
		m_subSkinChild.clear();
		destroyWidget();

	}


	WidgetPtr Widget::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _x, _y, _cx, _cy, _align, this, _name);
		m_widgetChild.push_back(widget);

		return widget;
	}

	WidgetPtr Widget::createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, char _align, const Ogre::String & _name)
	{
		Gui & gui = Gui::getInstance();
		return createWidget(_type, _skin, (int)(_x*gui.getWidth()), (int)(_y*gui.getHeight()), (int)(_cx*gui.getWidth()), (int)(_cy*gui.getHeight()), _align, _name);
	}

	BasisWidgetPtr  Widget::addSubSkin(const tagBasisWidgetInfo &_info, const String & _material)
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
			if (m_parent != null) m_parent->attach(_basis, true);
		}
	}

	void Widget::visible(bool _visible)
	{

		if (m_visible == _visible) return;
		m_visible = _visible;

		// ���� ������ �������������, �� �� ����������
		if (_visible && !m_show) return;

		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(m_visible);
	}

	void Widget::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
		// ���� ����� �� ������� �� �� ����������
		if (_show && !m_visible) return;

		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(m_show);
	}

	void Widget::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		// ��� ������� ��������� � �  �� ���������
		align(_cx, _cy, _update);
	}

	void Widget::align(int _cx, int _cy, bool _update)
	{
		if (m_parent == null) return;

		bool need_move = false;
		bool need_size = false;
		int x = m_x;
		int y = m_y;
		int cx = m_cx;
		int cy = m_cy;

		// �������������� ������������ 
		if (m_align & ALIGN_RIGHT) {
			if (m_align & ALIGN_LEFT) {
				// �����������
				cx = m_cx + (m_parent->width() - _cx);
				need_size = true;
			} else {
				// ������� �� ������� ����
				x = m_x + (m_parent->width() - _cx);
				need_move = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			x = (m_parent->width() - m_cx) / 2;
			need_move = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// �����������
				cy = m_cy + (m_parent->height() - _cy);
				need_size = true;
			} else {
				y = m_y + (m_parent->height() - _cy);
				need_move = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			y = (m_parent->height() - m_cy) / 2;
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
		bool margin = m_parent ? check_margin() : false;

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (check_outside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(m_x, m_y, _cx, _cy, m_margin || margin);
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(m_x, m_y, _cx, _cy, m_margin || margin);

		// ���������� ������� ���������
		m_margin = margin;

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
		bool margin = m_parent ? check_margin() : false;

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (check_outside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(_cx, _cy, m_margin || margin);
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(_cx, _cy, m_margin || margin);

		// ���������� ������� ���������
		m_margin = margin;

	}

	void Widget::update()
	{

		bool margin = m_parent ? check_margin() : false;

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (check_outside()) {

				// ��������
				visible(false);
				// ���������� ������� ���������
				m_margin = margin;
				return;
			}

		} else if (!m_margin) { // �� �� �������� � ���� ����������

			visible(true);
			// ��� ��� ���� ����� ���������� ���� ��� ��������
			for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->correct();

			return;

		}

		// ���������� ������� ���������
		m_margin = margin;

		// ���� ���� ��� �����, �� �������
		visible(true);

		// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->update();
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->update();

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

	void Widget::setTextAlign(char _align)
	{
		if (m_text != null) m_text->setTextAlign(_align);
	}

	void Widget::setAlpha(float _alpha)
	{
		m_alpha = _alpha;
		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->setAlpha(m_alpha);
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->setAlpha(m_alpha);
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
		StateInfo::const_iterator iter = m_stateInfo.find(_state);
		if (iter == m_stateInfo.end()) return;
		size_t index=0;
		// ������� �������� ��������
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			(*skin)->setUVSet(iter->second.m_offsets[index++]);
		}
		// ������ ���� ����� ���� ������
		if ((iter->second.color != Ogre::ColourValue::ZERO) && (m_text != null)) {
			m_text->setColour(iter->second.color);
		}
	}

	// ���������� ��������� �� ����� � ���� ����� ��������� � ������ (������������ �� LayerItemInfo)
	LayerItemInfoPtr Widget::findItem(int _x, int _y)
	{
		// ������������� ��������� ��������
		if (!m_enable) return this;
		// ��������� ���������
		if (!m_visible || !m_show || !check_point(_x, _y)) return 0;
		// ���������� � �������
		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) {
			LayerItemInfoPtr item = (*widget)->findItem(_x - m_x, _y - m_y);
			if (item) return item;
		}
		// ����������� ����
		return this;
	}

	// ������� ������ �������� ������� ��������
	void Widget::destroyWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < m_widgetChild.size(); index++) {
			WidgetPtr widget = m_widgetChild[index];
			if (_widget == widget) {
				// ������� ���� ���
				WidgetManager::getInstance().clearName(_widget);
				delete _widget;
				_widget = 0;

				// ������� �� ������
				m_widgetChild[index] = m_widgetChild[m_widgetChild.size()-1];
				m_widgetChild.pop_back();
				return;
			}
		}
	}

	// ������� ���� �����
	void Widget::destroyWidget()
	{
		for (WidgetChild::iterator iter = m_widgetChild.begin(); iter != m_widgetChild.end(); iter++) {
			WidgetPtr widget = *iter;
			// ������� ���� ���
			WidgetManager::getInstance().clearName(widget);
			// � �������
			delete widget;
		}
		m_widgetChild.clear();
	}

	void Widget::attachToOverlay(Ogre::Overlay * _overlay)
	{
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			if (!(*skin)->isText()) _overlay->add2D(static_cast<Ogre::OverlayContainer*>((*skin)->getOverlayElement()));
		}
	}

	void Widget::detachToOverlay(Ogre::Overlay * _overlay)
	{
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			if (!(*skin)->isText()) {
				PanelAlphaOverlayElement * element = static_cast<PanelAlphaOverlayElement*>((*skin)->getOverlayElement());
				_overlay->remove2D(element);
				// ���� ������� �������� ����
				element->setOverlay(0);
			}
		}
	}

} // namespace MyGUI