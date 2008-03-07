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
#include "MyGUI_SubWidgetTextInterface.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	Widget::Widget(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		CroppedRectangleInterface(IntCoord(_coord.point(), _info->getSize()), _align, _parent), // размер по скину
		mOwner(static_cast<Widget*>(_parent)),
		UserData(),
		mStateInfo(_info->getStateInfo()),
		mMaskPeekInfo(_info->getMask()),
		mText(null),
		mEnabled(true),
		mVisible(true),
		mAlpha(ALPHA_MIN),
		mColour(Ogre::ColourValue::White),
		mName(_name),
		mCountSharedOverlay(0)
	{
		// имя отсылателя сообщений
		mWidgetEventSender = this;

		// подсчитаываем колличество оверлеев для объединения
		for (VectorSubWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			if (SubWidgetManager::getInstance().isSharedOverlay(*iter)) mCountSharedOverlay++;
		}

		// загружаем кирпичики виджета
		size_t id = 0;
		for (VectorSubWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			addSubSkin(*iter, _info->getMaterial(), id);
		}

		// парсим свойства
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

		// этот стиль есть всегда, даже если создатель не хотел его
		setState("normal");
		setAlpha(ALPHA_MAX);
		// альфа отца
		//if ( (mParent != null) && (static_cast<WidgetPtr>(mParent)->getAlpha() != ALPHA_MAX) ) setAlpha(static_cast<WidgetPtr>(mParent)->getAlpha());

		// создаем детей
		const VectorChildSkinInfo& child = _info->getChild();
		for (VectorChildSkinInfo::const_iterator iter=child.begin(); iter!=child.end(); ++iter) {
			WidgetPtr widget = createWidgetT(iter->type, iter->skin, iter->coord, iter->align);
			widget->_setInternalString(iter->name);
			// заполняем UserString пропертями
			for (MapString::const_iterator prop=iter->params.begin(); prop!=iter->params.end(); ++prop) {
				widget->setUserString(prop->first, prop->second);
			}
		}

		// а вот теперь нормальный размер
		setSize(_coord.size());
	}

	Widget::~Widget()
	{
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
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

	CroppedRectanglePtr  Widget::addSubSkin(const SubWidgetInfo& _info, const Ogre::String& _material, size_t & _id)
	{
		CroppedRectanglePtr sub = SubWidgetManager::getInstance().createSubWidget(_info, _material, this, _id);
		// если это скин текста, то запоминаем
		if (sub->_isText()) mText = static_cast<SubWidgetTextPtr>(sub);
		// добавляем в общий список
		mSubSkinChild.push_back(sub);
		return sub;
	}

	void Widget::_attachChild(CroppedRectanglePtr _basis, bool _child)
	{
		if (_child) {
			// это к нам текст хочет прилипиться
			MYGUI_ASSERT(mSubSkinChild.size() > 0, "Widget must have at least one subskin");
			mSubSkinChild[0]->_attachChild(_basis, true);
		} else {
			// нет не к нам, а к нашему отцу
			if (mParent != null) mParent->_attachChild(_basis, true);
		}
	}

	void Widget::_setVisible(bool _visible)
	{
		if (mVisible == _visible) return;
		mVisible = _visible;

		// если скрыто пользователем, то не показываем
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
		// если вышло за границу то не показываем
		if (mShow && !mVisible) return;

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->show();
	}

	void Widget::hide()
	{
		if (false == mShow) return;
		mShow = false;
		// если вышло за границу то не показываем
		if (mShow && !mVisible) return;

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->hide();
	}

	void Widget::_setAlign(const IntCoord& _coord, bool _update)
	{
		// для виджета изменение х у  не меняються
		_setAlign(_coord.size(), _update);
	}

	void Widget::_setAlign(const IntSize& _size, bool _update)
	{
		if (mParent == null) return;

		bool need_move = false;
		bool need_size = false;
		IntCoord coord = mCoord;

		// первоначальное выравнивание
		if (IS_ALIGN_RIGHT(mAlign)) {
			if (IS_ALIGN_LEFT(mAlign)) {
				// растягиваем
				coord.width = mCoord.width + (mParent->getWidth() - _size.width);
				need_size = true;
			} else {
				// двигаем по правому краю
				coord.left = mCoord.left + (mParent->getWidth() - _size.width);
				need_move = true;
			}

		} else if (false == IS_ALIGN_LEFT(mAlign)) {
			// выравнивание по горизонтали без растяжения
			coord.left = (mParent->getWidth() - mCoord.width) / 2;
			need_move = true;
		}

		if (IS_ALIGN_BOTTOM(mAlign)) {
			if (IS_ALIGN_TOP(mAlign)) {
				// растягиваем
				coord.height = mCoord.height + (mParent->getHeight() - _size.height);
				need_size = true;
			} else {
				coord.top = mCoord.top + (mParent->getHeight() - _size.height);
				need_move = true;
			}
		} else if (false == IS_ALIGN_TOP(mAlign)) {
			// выравнивание по вертикали без растяжения
			coord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) setPosition(coord);
			else setPosition(coord.point());
		} else if (need_size) {
			setSize(coord.size());
		} else _updateView(); // только если не вызвано передвижение и сайз

	}

	void Widget::setPosition(const IntPoint& _pos)
	{
		// а вот теперь запоминаем новые координаты
		mCoord = _pos;

		_updateView();
	}

	void Widget::setPosition(const IntCoord& _coord)
	{
		// устанавливаем новую координату а старую пускаем в расчеты
		IntCoord old = mCoord;
		mCoord = _coord;

		bool show = true;

		// обновляем выравнивание
		bool margin = mParent ? _checkMargin() : false;

		if (margin) {
			// проверка на полный выход за границу
			if (_checkOutside()) {
				// скрываем
				show = false;
			}
		}

		_setVisible(show);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->_setAlign(old, mIsMargin || margin);

		// запоминаем текущее состояние
		mIsMargin = margin;

	}

	void Widget::setSize(const IntSize& _size)
	{
		// устанавливаем новую координату а старую пускаем в расчеты
		IntSize old = mCoord.size();
		mCoord = _size;

		bool show = true;

		// обновляем выравнивание
		bool margin = mParent ? _checkMargin() : false;

		if (margin) {
			// проверка на полный выход за границу
			if (_checkOutside()) {
				// скрываем
				show = false;
			}
		}

		_setVisible(show);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->_setAlign(old, mIsMargin || margin);

		// запоминаем текущее состояние
		mIsMargin = margin;

	}

	void Widget::_updateView()
	{

		bool margin = mParent ? _checkMargin() : false;

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (_checkOutside()) {

				// скрываем
				_setVisible(false);
				// запоминаем текущее состояние
				mIsMargin = margin;
				return;
			}

		} else if (false == mIsMargin) { // мы не обрезаны и были нормальные

			_setVisible(true);
			// для тех кому нужно подправить себя при движении
			for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->_correctView();

			return;

		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		// если скин был скрыт, то покажем
		_setVisible(true);

		// обновляем наших детей, а они уже решат обновлять ли своих детей
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
		// сначала сдвигаем текстуры
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) {
			CroppedRectanglePtr & info = (*skin);
			if (false == info->_isText()) info->_setUVSet(iter->second.offsets[index++]);
		}
		// теперь если нужно цвет текста
		if ((iter->second.colour != Ogre::ColourValue::ZERO) && (mText != null)) {
			mText->setColour(iter->second.colour);
		}
	}

	// возвращает указатель на айтем в этой точке попадание в виджет (наследуеться от LayerItemInfo)
	LayerItemInfoPtr Widget::findItem(int _left, int _top)
	{
		// проверяем попадание
		if (!mVisible || !mShow || !_checkPoint(_left, _top)) return null;
		// если есть маска, проверяем еще и по маске
		if ((false == mMaskPeekInfo.empty()) &&
			(false == mMaskPeekInfo.peek(IntPoint(_left, _top)-mCoord.point(), mCoord))) return null;
		// останавливаем каскадную проверку
		if (mEnabled) {
			// спрашиваем у детишек
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) {
				LayerItemInfoPtr item = (*widget)->findItem(_left - mCoord.left, _top - mCoord.top);
				if (item != null) return item;
			}
		}
		// непослушные дети
		return this;
	}

	void Widget::setEnabled(bool _enabled, bool _silent)
	{
		mEnabled = _enabled;
		if (_silent) return;

		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
		{
			(*iter)->setEnabled(_enabled);
		}

		if (mEnabled) setState("normal");
		else {
			setState("disable");
			InputManager::getInstance()._unlinkWidget(this);
		}
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
				// удаляем только шаред главный
				if (null != element) {
					_overlay->remove2D(element);
					// пока вручную обнуляем отца
					element->setOverlay(0);
				}
			}
		}
	}

	// удяляет только негодных батюшке государю
	void Widget::_destroyChildWidget(WidgetPtr _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if(iter != mWidgetChild.end())
		{
			delete *iter;
			// удаляем из списка
			*iter = mWidgetChild.back();
			mWidgetChild.pop_back();
			return;
		}
		MYGUI_EXCEPT("Widget not found");
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
		if (null == mText) return IntSize();
		return mText->getTextSize();
	}

	IntSize Widget::getTextSize(const Ogre::DisplayString& _text)
	{
		if (null == mText) return IntSize();
		return mText->getTextSize(_text);
	}

	IntCoord Widget::getTextCoord()
	{
		if (null == mText) return IntCoord();
		return mText->getCoord();
	}

	void Widget::setAlpha(float _alpha)
	{
		if (mAlpha == _alpha) return;
		mAlpha = _alpha;
		if (null != mParent) mRealAlpha = mAlpha * static_cast<Widget*>(mParent)->_getRealAlpha();
		else mRealAlpha = mAlpha;

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->_updateAlpha();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->setAlpha(mRealAlpha);
	}

	void Widget::_updateAlpha()
	{
		MYGUI_DEBUG_ASSERT(null != mParent, "parent must be");
		mRealAlpha = mAlpha * static_cast<Widget*>(mParent)->_getRealAlpha();
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); widget++) (*widget)->_updateAlpha();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); skin++) (*skin)->setAlpha(mRealAlpha);
	}

} // namespace MyGUI
