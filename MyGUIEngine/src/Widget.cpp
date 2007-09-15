
#include "Widget.h"
#include "BasisWidgetManager.h"
#include "WidgetManager.h"
#include "WidgetParser.h"
#include "Gui.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<Widget> WidgetFactoryInstance; }
	// парсер команд для Widget
	namespace parser { WidgetParser WidgetParserInstance; }

	Widget::Widget(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		BasisWidget(_x, _y, _info->getSize().width, _info->getSize().height, _align, _parent), // размер по скину
		m_text(0),
		m_visible(true),
		m_enable(true),
		m_alpha(1.0),
		m_color(1.0, 1.0, 1.0, 1.0),
		m_stateInfo(_info->getStateInfo()),
		m_name(_name)
	{
		// имя отсылателя сообщений
		m_widgetEventSender = this;

		// загружаем кирпичики виджета
		for (BasisInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			addSubSkin(*iter, _info->getMaterial());
		}

		// парсим свойства
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

		// этот стиль есть всегда, даже если создатель не хотел его
		setState("normal");
		// а вот теперь нормальный размер
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
			if (m_parent != null) m_parent->attach(_basis, true);
		}
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
		if (m_parent == null) return;

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
		bool margin = m_parent ? check_margin() : false;

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
		// меняем координаты местами
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		bool show = true;

		// обновляем выравнивание
		bool margin = m_parent ? check_margin() : false;

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

		bool margin = m_parent ? check_margin() : false;

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

			visible(true);
			// для тех кому нужно подправить себя при движении
			for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->correct();

			return;

		}

		// запоминаем текущее состояние
		m_margin = margin;

		// если скин был скрыт, то покажем
		visible(true);

		// обновляем наших детей, а они уже решат обновлять ли своих детей
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
		// сначала сдвигаем текстуры
		for (BasisChild::iterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			(*skin)->setUVSet(iter->second.m_offsets[index++]);
		}
		// теперь если нужно цвет текста
		if ((iter->second.color != Ogre::ColourValue::ZERO) && (m_text != null)) {
			m_text->setColour(iter->second.color);
		}
	}

	// возвращает указатель на айтем в этой точке попадание в виджет (наследуеться от LayerItemInfo)
	LayerItemInfoPtr Widget::findItem(int _x, int _y)
	{
		// останавливаем каскадную проверку
		if (!m_enable) return this;
		// проверяем попадание
		if (!m_visible || !m_show || !check_point(_x, _y)) return 0;
		// спрашиваем у детишек
		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) {
			LayerItemInfoPtr item = (*widget)->findItem(_x - m_x, _y - m_y);
			if (item) return item;
		}
		// непослушные дети
		return this;
	}

	// удяляет только негодных батюшке государю
	void Widget::destroyWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < m_widgetChild.size(); index++) {
			WidgetPtr widget = m_widgetChild[index];
			if (_widget == widget) {
				// удаляем свое имя
				WidgetManager::getInstance().clearName(_widget);
				delete _widget;
				_widget = 0;

				// удаляем из списка
				m_widgetChild[index] = m_widgetChild[m_widgetChild.size()-1];
				m_widgetChild.pop_back();
				return;
			}
		}
	}

	// удаляет всех детей
	void Widget::destroyWidget()
	{
		for (WidgetChild::iterator iter = m_widgetChild.begin(); iter != m_widgetChild.end(); iter++) {
			WidgetPtr widget = *iter;
			// удаляем свое имя
			WidgetManager::getInstance().clearName(widget);
			// и удаляем
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
				// пока вручную обнуляем отца
				element->setOverlay(0);
			}
		}
	}

} // namespace MyGUI