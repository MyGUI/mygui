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

namespace MyGUI
{

	Widget::Widget(int _left, int _top, int _width, int _height, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		BasisWidget(_left, _top, _info->getSize().width, _info->getSize().height, _align, _parent), // размер по скину
		mText(0),
		mVisible(true),
		mEnabled(true),
		mAlpha(1.0),
		mColour(1.0, 1.0, 1.0, 1.0),
		mStateInfo(_info->getStateInfo()),
		mName(_name),
		mCountSharedOverlay(0)
	{
		// имя отсылателя сообщений
		mWidgetEventSender = this;

		// подсчитаываем колличество оверлеев для объединения
		for (VectorBasisWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			if (BasisWidgetManager::getInstance().isSharedOverlay(*iter)) mCountSharedOverlay++;
		}

		// загружаем кирпичики виджета
		size_t id = 0;
		for (VectorBasisWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			addSubSkin(*iter, _info->getMaterial(), id);
		}

		// парсим свойства
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

		// этот стиль есть всегда, даже если создатель не хотел его
		setState("normal");
		// а вот теперь нормальный размер
		size(_width, _height);
		// альфа отца
		if ( (mParent != null) && (static_cast<WidgetPtr>(mParent)->getAlpha() != 1.0f) ) setAlpha(static_cast<WidgetPtr>(mParent)->getAlpha());
		// и все перерисовываем
		update();
	}

	Widget::~Widget()
	{
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			delete (*skin);
		}
		mSubSkinChild.clear();
		_destroyAllChildWidget();

		// удаляем себя из списка рассылки, но мы там быть недолжны
		if (Gui::getInstance().removeFrameListener(this)) {
			MYGUI_ERROR("widget is not remove from frame listeners");
		}
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

	BasisWidgetPtr  Widget::addSubSkin(const BasisWidgetInfo& _info, const Ogre::String& _material, size_t & _id)
	{
		BasisWidgetPtr sub = BasisWidgetManager::getInstance().createBasisWidget(_info, _material, this, _id);
		// если это скин текста, то запоминаем
		if (sub->isText()) mText = sub;
		// добавляем в общий список
		mSubSkinChild.push_back(sub);
		return sub;
	}

	void Widget::attach(BasisWidgetPtr _basis, bool _child)
	{
		if (_child) {
			// это к нам текст хочет прилипиться
			MYGUI_ASSERT(mSubSkinChild.size() > 0);
			mSubSkinChild[0]->attach(_basis, true);
		} else {
			// нет не к нам, а к нашему отцу
			if (mParent != null) mParent->attach(_basis, true);
		}
	}

	void Widget::_setVisible(bool _visible)
	{
		if (mVisible == _visible) return;
		mVisible = _visible;

		// если скрыто пользователем, то не показываем
		if (mVisible && !mShow) return;

		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			if (mVisible) (*skin)->show();
			else (*skin)->hide();
		}
	}

	void Widget::show()
	{
		if (mShow) return;
		mShow = true;
		// если вышло за границу то не показываем
		if (mShow && !mVisible) return;

		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->show();
	}

	void Widget::hide()
	{
		if (false == mShow) return;
		mShow = false;
		// если вышло за границу то не показываем
		if (mShow && !mVisible) return;

		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->hide();
	}

	void Widget::align(int _left, int _top, int _width, int _height, bool _update)
	{
		// для виджета изменение х у  не меняються
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

		// первоначальное выравнивание 
		if (mAlign & ALIGN_RIGHT) {
			if (mAlign & ALIGN_LEFT) {
				// растягиваем
				cx = mWidth + (mParent->getWidth() - _width);
				need_size = true;
			} else {
				// двигаем по правому краю
				x = mLeft + (mParent->getWidth() - _width);
				need_move = true;
			}

		} else if (!(mAlign & ALIGN_LEFT)) {
			// выравнивание по горизонтали без растяжения
			x = (mParent->getWidth() - mWidth) / 2;
			need_move = true;
		}

		if (mAlign & ALIGN_BOTTOM) {
			if (mAlign & ALIGN_TOP) {
				// растягиваем
				cy = mHeight + (mParent->getHeight() - _height);
				need_size = true;
			} else {
				y = mTop + (mParent->getHeight() - _height);
				need_move = true;
			}
		} else if (!(mAlign & ALIGN_TOP)) {
			// выравнивание по вертикали без растяжения
			y = (mParent->getHeight() - mHeight) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) move(x, y, cx, cy);
			else move(x, y);
		} else if (need_size) {
			size(cx, cy);
		} else update(); // только если не вызвано передвижение и сайз

	}


	void Widget::move(int _left, int _top)
	{
		// а вот теперь запоминаем новые координаты
		mLeft = _left;
		mTop = _top;

		update();
	}

	void Widget::move(int _left, int _top, int _width, int _height)
	{
		// а вот теперь запоминаем новые координаты
		mLeft = _left;
		mTop = _top;

		// меняем координаты местами
		int tmp = mWidth;
		mWidth = _width;
		_width = tmp;

		tmp = mHeight;
		mHeight = _height;
		_height = tmp;

		bool show = true;

		// обновляем выравнивание
		bool margin = mParent ? checkMargin() : false;

		if (margin) {
			// проверка на полный выход за границу
			if (checkOutside()) {
				// скрываем
				show = false;
			}
		}

		_setVisible(show);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->align(mLeft, mTop, _width, _height, mMargin || margin);
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->align(mLeft, mTop, _width, _height, mMargin || margin);

		// запоминаем текущее состояние
		mMargin = margin;

	}

	void Widget::size(int _width, int _height)
	{
		// меняем координаты местами
		int tmp = mWidth;
		mWidth = _width;
		_width = tmp;

		tmp = mHeight;
		mHeight = _height;
		_height = tmp;

		bool show = true;

		// обновляем выравнивание
		bool margin = mParent ? checkMargin() : false;

		if (margin) {
			// проверка на полный выход за границу
			if (checkOutside()) {
				// скрываем
				show = false;
			}
		}

		_setVisible(show);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->align(_width, _height, mMargin || margin);
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->align(_width, _height, mMargin || margin);

		// запоминаем текущее состояние
		mMargin = margin;

	}

	void Widget::update()
	{

		bool margin = mParent ? checkMargin() : false;

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (checkOutside()) {

				// скрываем
				_setVisible(false);
				// запоминаем текущее состояние
				mMargin = margin;
				return;
			}

		} else if (!mMargin) { // мы не обрезаны и были нормальные

			_setVisible(true);
			// для тех кому нужно подправить себя при движении
			for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->correct();

			return;

		}

		// запоминаем текущее состояние
		mMargin = margin;

		// если скин был скрыт, то покажем
		_setVisible(true);

		// обновляем наших детей, а они уже решат обновлять ли своих детей
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
		// сначала сдвигаем текстуры
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			(*skin)->setUVSet(iter->second.offsets[index++]);
		}
		// теперь если нужно цвет текста
		if ((iter->second.color != Ogre::ColourValue::ZERO) && (mText != null)) {
			mText->setColour(iter->second.color);
		}
	}

	// возвращает указатель на айтем в этой точке попадание в виджет (наследуеться от LayerItemInfo)
	LayerItemInfoPtr Widget::findItem(int _left, int _top)
	{
		// проверяем попадание
		if (!mVisible || !mShow || !checkPoint(_left, _top)) return 0;
		// останавливаем каскадную проверку
		if (!mEnabled) return this;
		// спрашиваем у детишек
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) {
			LayerItemInfoPtr item = (*widget)->findItem(_left - mLeft, _top - mTop);
			if (item != null) return item;
		}
		// непослушные дети
		return this;
	}

	void Widget::setEnabled(bool _enabled)
	{
		mEnabled = _enabled;
		InputManager::getInstance().widgetUnlink(this);
	}

	void Widget::attachToOverlay(Ogre::Overlay * _overlay)
	{
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			if (false == (*skin)->isText()) {
				Ogre::OverlayContainer * element = static_cast<Ogre::OverlayContainer*>((*skin)->getOverlayElement());
				if (null != element) _overlay->add2D(element);
			}
		}
	}

	void Widget::detachToOverlay(Ogre::Overlay * _overlay)
	{
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			if (!(*skin)->isText()) {
				SharedPanelAlphaOverlayElement * element = static_cast<SharedPanelAlphaOverlayElement*>((*skin)->getOverlayElement());
				// удаляем только шаред главный
				if (null != element) {
					_overlay->remove2D(element);
					// пока вручную обнуляем отца
					element->setOverlay(0);
				}
			}
		}
	}

	// вспомогательный метод для распарсивания сабвиджетофф
	WidgetPtr Widget::parseSubWidget(const MapString & _param, const std::string & _type, const std::string & _skin, const std::string & _offset, const std::string & _align, const IntSize &_size)
	{
		// парсим заголовок
		MapString::const_iterator iter = _param.find(_skin);
		if ( (iter != _param.end()) && (! iter->second.empty()) ) {
			// сохраняем скин
			std::string skin = iter->second;
			FloatRect offset;
			Align align;
			// смещение в скине
			iter = _param.find(_offset);
			if (iter != _param.end()) offset = util::parseFloatRect(iter->second);
			else offset.clear();
			// выравнивание скина
			iter = _param.find(_align);
			if (iter != _param.end()) align = SkinManager::getInstance().parseAlign(iter->second);
			else align = ALIGN_NONE;

			offset = WidgetManager::convertOffset(offset, align, _size, mWidth, mHeight);
			return createWidget(_type, skin, offset.left, offset.top, offset.right, offset.bottom, align);
			
		}
		return null;
	}

	// удяляет только негодных батюшке государю
	void Widget::_destroyChildWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < mWidgetChild.size(); index++) {
			WidgetPtr widget = mWidgetChild[index];
			if (_widget == widget) {

				delete _widget;
				_widget = null;

				// удаляем из списка
				mWidgetChild[index] = mWidgetChild[mWidgetChild.size()-1];
				mWidgetChild.pop_back();
				return;
			}
		}
		MYGUI_EXCEPT("Widget is not find");
	}

	// удаляет всех детей
	void Widget::_destroyAllChildWidget()
	{
		while (false == mWidgetChild.empty()) {
			// отсылаем первый, так как он быстрее найдется в массиве
			// а удаление в векторе производится перестановкой, т.е. быстро
			WidgetPtr widget = mWidgetChild.front();
			WidgetManager::getInstance().destroyWidget(widget);
		}
	}
	// возвращаем колличество сабскинов без учета текста
	size_t Widget::_getCountSharedOverlay()
	{
		return mCountSharedOverlay;
	}

	Ogre::OverlayElement* Widget::_getSharedOverlay()
	{
		for (VectorBasisWidgetPtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			Ogre::OverlayElement* element = (*skin)->_getSharedOverlay();
			if (null != element) return element;
		}
		return null;
	}

} // namespace MyGUI