
#include "Widget.h"
#include "debugOut.h"
#include "BasisWidgetManager.h"
#include "WidgetManager.h"
#include "ParserWidget.h"

namespace widget
{
	namespace helper
	{
		// создаем фабрику для этого виджета
		WidgetFactory<Widget> factory_Widget;
		// парсер команд для Widget
		ParserWidget parser_Widget;
	}

	Widget::Widget(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		BasisWidget(_x, _y, _info->width(), _info->height(), _align, _parent), // размер по скину
		m_text(0),
		m_visible(true),
		m_alpha(1.0),
		m_color(1.0, 1.0, 1.0, 1.0),
		m_stateInfo(_info->getStateInfo()),
		m_name(_name)
	{

		// загружаем кирпичика виджета
		for (BasisInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			addSubSkin(*iter, _info->getMaterial());
		}

		// парсим свойства
		const SkinParam & param = _info->getParams();
		SkinParam::const_iterator iter = param.find("FontName");
		if (iter != param.end()) setFontName(iter->second);
		iter = param.find("FontHeight");
		if (iter != param.end()) setCharHeight(parseInt(iter->second));

		// этот стиль есть всегда, даже если создатель не хотел его
		setState("normal");
		// а вот теперь нормальный размер
		size(_cx, _cy);
		update();
	}

		WidgetPtr Widget::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _x, _y, _cx, _cy, _align, this, _name);
		m_widgetChild.push_back(widget);

		return widget;
	}

	BasisWidgetPtr  Widget::addSubSkin(const tagBasisWidgetInfo &_info, const String & _material)
	{
		BasisWidgetPtr sub = BasisWidgetManager::getInstance().createBasisWidget(_info, _material, this);
		// если это скин текста, то запоминаем
		if (sub->isText()) m_text = sub;
		// добавляем в общий список
		m_subSkinChild.push_back(sub);
		return sub;
	}

	void Widget::attach(BasisWidgetPtr _basis, bool _child)
	{
		if (_child) {
			// это к нам текст хочет прилипиться
			assert(m_subSkinChild.size() > 0);
			m_subSkinChild[0]->attach(_basis, true);
		} else {
			// нет не к нам, а к нашему отцу
			m_parent->attach(_basis, true);
		}
	}

	OverlayElementPtr Widget::getOverlayElement()
	{
		if (m_subSkinChild.empty()) return 0;
		return m_subSkinChild[0]->getOverlayElement();
	}

	void Widget::visible(bool _visible)
	{

		if (m_visible == _visible) return;
		m_visible = _visible;

		// если скрыто пользователем, то не показываем
		if (_visible && !m_show) return;

		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(m_visible);
	}

	void Widget::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
		// если вышло за границу то не показываем
		if (_show && !m_visible) return;

		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(m_show);
	}

	void Widget::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		// для виджета изменение х у  не меняються
		align(_cx, _cy, _update);
	}

	void Widget::align(int _cx, int _cy, bool _update)
	{
		if (!m_parent) return;

		bool need_move = false;
		bool need_size = false;
		int x = m_x;
		int y = m_y;
		int cx = m_cx;
		int cy = m_cy;

		// первоначальное выравнивание 
		if (m_align & ALIGN_RIGHT) {
			if (m_align & ALIGN_LEFT) {
				// растягиваем
				cx = m_cx + (m_parent->width() - _cx);
				need_size = true;
			} else {
				// двигаем по правому краю
				x = m_x + (m_parent->width() - _cx);
				need_move = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// выравнивание по горизонтали без растяжения
			x = (m_parent->width() - m_cx) / 2;
			need_move = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// растягиваем
				cy = m_cy + (m_parent->height() - _cy);
				need_size = true;
			} else {
				y = m_y + (m_parent->height() - _cy);
				need_move = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// выравнивание по вертикали без растяжения
			y = (m_parent->height() - m_cy) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) move(x, y, cx, cy);
			else move(x, y);
		} else if (need_size) {
			size(cx, cy);
		} else update(); // только если не вызвано передвижение и сайз

	}


	void Widget::move(int _x, int _y)
	{

		// а вот теперь запоминаем новые координаты
		m_x = _x;
		m_y = _y;

		update();
	}

	void Widget::move(int _x, int _y, int _cx, int _cy)
	{

		if (!m_parent) return;

		// а вот теперь запоминаем новые координаты
		m_x = _x;
		m_y = _y;

		// меняем координаты местами
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		bool show = true;

		// обновляем выравнивание
		bool margin = check_margin();

		if (margin) {
			// проверка на полный выход за границу
			if (check_outside()) {
				// скрываем
				show = false;
			}
		}

		visible(show);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(m_x, m_y, _cx, _cy, m_margin || margin);
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(m_x, m_y, _cx, _cy, m_margin || margin);

		// запоминаем текущее состояние
		m_margin = margin;

	}

	void Widget::size(int _cx, int _cy)
	{

		if (!m_parent) return;

		// меняем координаты местами
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		bool show = true;

		// обновляем выравнивание
		bool margin = check_margin();

		if (margin) {
			// проверка на полный выход за границу
			if (check_outside()) {
				// скрываем
				show = false;
			}
		}

		visible(show);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(_cx, _cy, m_margin || margin);
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(_cx, _cy, m_margin || margin);

		// запоминаем текущее состояние
		m_margin = margin;

	}

	void Widget::update()
	{

		if (!m_parent) return;

		bool margin = check_margin();

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (check_outside()) {

				// скрываем
				visible(false);
				// запоминаем текущее состояние
				m_margin = margin;
				return;

			}

		} else if (!m_margin) { // мы не обрезаны и были нормальные

			// запоминаем текущее состояние
			m_margin = margin;

			// для тех кому нужно подправить себя при движении
			for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->correct();

			// скрываем
			visible(true);

			return;

		}

		// запоминаем текущее состояние
		m_margin = margin;

		// обновляем наших детей, а они уже решат обновлять ли своих детей
		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->update();
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->update();

		// если скин был скрыт, то покажем
		visible(true);

	}

	void Widget::setCaption(const Ogre::DisplayString & _caption)
	{
		if (!m_text) return;
		m_text->setCaption(_caption);
	}

	const Ogre::DisplayString & Widget::getCaption()
	{
		if (!m_text) return BasisWidget::getCaption();
		return m_text->getCaption();
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

	void Widget::setCharHeight(Ogre::ushort _height)
	{
		if (!m_text) return;
		m_text->setCharHeight(_height);
	}

	Ogre::ushort Widget::getCharHeight()
	{
		if (!m_text) return 0;
		return m_text->getCharHeight();
	}

	void Widget::setState(const Ogre::String & _state)
	{
		StateInfo::const_iterator iter = m_stateInfo.find(_state);
		if (iter == m_stateInfo.end()) return;
		size_t index=0;
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			(*skin)->setUVSet(iter->second.m_offsets[index++]);
		}
	}

	void Widget::detach(WidgetPtr _child)
	{
		for (size_t index = 0; index != m_widgetChild.size(); index++) {
			if (m_widgetChild[index] == _child) {
				m_widgetChild[index] = m_widgetChild[m_widgetChild.size()-1] ;
				m_widgetChild.pop_back();
				break;
			}
		}
	}

	Widget::~Widget()
	{
		// отписывает себя от отца
		static_cast<WidgetPtr>(m_parent)->detach(this);
		// удаляем детей
		while (m_widgetChild.size() > 0) {
			// при удалении виджет сам себя отпишет из этого списка
			WidgetManager::getInstance().destroyWidget(m_widgetChild[0]);
		};
		// удаляем саб скины
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			delete (*skin);
		}
		m_subSkinChild.clear();

	}

} // namespace widget