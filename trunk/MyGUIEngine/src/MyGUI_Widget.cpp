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
#include "MyGUI_LayerManager.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_SubWidgetTextInterface.h"

namespace MyGUI
{

	Ogre::String Widget::WidgetTypeName = "Widget";

	const float WIDGET_TOOLTIP_TIMEOUT = 1;

	Widget::Widget(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		CroppedRectangleInterface(IntCoord(_coord.point(), _info->getSize()), _align, _parent), // размер по скину
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
		mWidgetCreator(_creator),
		mInheritsAlpha(true),
		mNeedKeyFocus(false),
		mNeedMouseFocus(true),
		mNeedDragDrop(false),
		mWidgetClient(null),
		m_toolTipEnable(false),
		m_toolTipCurrentTime(0),
		m_toolTipVisible(false)
	{
		// корректируем абсолютные координаты
		mAbsolutePosition = _coord.point();
		if (null != mParent) mAbsolutePosition += mParent->getAbsolutePosition();

		// имя отсылателя сообщений
		mWidgetEventSender = this;

		// загружаем кирпичики виджета
		SubWidgetManager & manager = SubWidgetManager::getInstance();
		for (VectorSubWidgetInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); ++iter) {
			CroppedRectangleInterface * sub = manager.createSubWidget(*iter, this);
			mSubSkinChild.push_back(sub);
			if (sub->_isText()) mText = static_cast<SubWidgetTextInterfacePtr>(sub);
			else if (null == mMainSkin) mMainSkin = sub;
		}

		if (false == isRootWidget()) {
			// если отец уже приаттачен, то и мы аттачимся ??? проверить что уже аттачиться при сет текстуре
			if ((null != getParent()->getLayerItemKeeper())) _attachToLayerItemKeeper(getParent()->getLayerItemKeeper());

			// проверяем наследуемую скрытость
			if ((!mParent->isShow()) || (!getParent()->_isInheritedShow())) {
				mInheritedShow = false;
				// скрываем только саб скины, детей у нас еще нет
				for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->hide();
			}
		}

		// этот стиль есть всегда, даже если создатель не хотел его
		setState("normal");

		// парсим свойства
		const MapString & param = _info->getParams();
		if (false == param.empty()) {
			MapString::const_iterator iter = param.find("FontName");
			if (iter != param.end()) setFontName(iter->second);
			iter = param.find("FontHeight");
			if (iter != param.end()) setFontHeight(utility::parseInt(iter->second));
			iter = param.find("NeedKey");
			if (iter != param.end()) setNeedKeyFocus(iter->second == "true");
			iter = param.find("NeedMouse");
			if (iter != param.end()) setNeedMouseFocus(iter->second == "true");
			iter = param.find("AlignText");
			if (iter != param.end()) setTextAlign(SkinManager::parseAlign(iter->second));
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

		// выставляем альфу, корректировка по отцу автоматически
		setAlpha(ALPHA_MAX);

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
		_detachFromLayerItemKeeper();

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
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
		return createWidgetT(_type, _skin, Gui::getInstance().convertRelativeToInt(_coord, this), _align, _name);
	}

	void Widget::_setAlign(const IntCoord& _coord, bool _update)
	{
		// для виджета изменение х у  не меняються
		_setAlign(_coord.size(), _update);
	}

	void Widget::_setAlign(const IntSize& _size, bool _update)
	{
		if (null == mParent) return;

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

	void Widget::_updateView()
	{

		bool margin = mParent ? _checkMargin() : false;

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (_checkOutside()) {

				// запоминаем текущее состояние
				mIsMargin = margin;

				// скрываем
				_setVisible(false);

				// для тех кому нужно подправить себя при движении
				//for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

				// вся иерархия должна быть проверенна
				for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateView();

				return;
			}

		} else if (false == mIsMargin) { // мы не обрезаны и были нормальные

			// запоминаем текущее состояние
			//mIsMargin = margin;

			//_setVisible(true);
			// для тех кому нужно подправить себя при движении
			for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

			return;

		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		// если скин был скрыт, то покажем
		_setVisible(true);

		// обновляем наших детей, а они уже решат обновлять ли своих детей
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateView();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

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
		return ALIGN_DEFAULT;
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
		// сначала сдвигаем текстуры
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			CroppedRectanglePtr & info = (*skin);
			if (false == info->_isText()) info->_setUVSet(iter->second.offsets[index++]);
			//else info->_correctView();//???
		}
		// теперь если нужно цвет текста
		if (mText != null) {
			if (iter->second.colour != Ogre::ColourValue::ZERO) {
				mText->setColour(iter->second.colour);
			}
			mText->setShiftText(iter->second.shift);
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

	// удяляет неудачника
	void Widget::_destroyChildWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(null != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end()) {

			// сохраняем указатель
			MyGUI::WidgetPtr widget = *iter;

			// удаляем из списка
			*iter = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			WidgetManager::getInstance().unlinkFromUnlinkers(_widget);

			// непосредственное удаление
			_deleteWidget(widget);
		}
		else MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
	}

	// удаляет всех детей
	void Widget::_destroyAllChildWidget()
	{
		WidgetManager & manager = WidgetManager::getInstance();
		while (false == mWidgetChild.empty()) {

			// сразу себя отписывем, иначе вложенной удаление убивает все
			WidgetPtr widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			manager.unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			delete widget;
		}
	}

	IntCoord Widget::getClientCoord()
	{
		if (mWidgetClient != null) return mWidgetClient->getCoord();
		return IntCoord(0, 0, mCoord.width, mCoord.height);
	}

	VectorWidgetPtr Widget::getChilds()
	{
		return mWidgetChild;
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
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->setAlpha(mRealAlpha);
	}

	void Widget::_updateAlpha()
	{
		MYGUI_DEBUG_ASSERT(null != mParent, "Widget must have parent");
		mRealAlpha = mAlpha * (mInheritsAlpha ? static_cast<Widget*>(mParent)->_getRealAlpha() : ALPHA_MAX);
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAlpha();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->setAlpha(mRealAlpha);
	}

	LayerItem * Widget::_findLayerItem(int _left, int _top)
	{
		// проверяем попадание
		if (!mVisible || !mShow || !mNeedMouseFocus || !_checkPoint(_left, _top)) return null;
		// если есть маска, проверяем еще и по маске
		if ((false == mMaskPeekInfo.empty()) &&
			(false == mMaskPeekInfo.peek(IntPoint(_left, _top)-mCoord.point(), mCoord))) return null;
		// останавливаем каскадную проверку
		if (mEnabled) {
			// спрашиваем у детишек
			for (VectorWidgetPtr::reverse_iterator widget= mWidgetChild.rbegin(); widget != mWidgetChild.rend(); ++widget) {
				LayerItem * item = (*widget)->_findLayerItem(_left - mCoord.left, _top - mCoord.top);
				if (item != null) return item;
			}
		}
		// непослушные дети
		return this;
	}

	void Widget::_updateAbsolutePoint()
	{
		mAbsolutePosition = mParent->getAbsolutePosition() + mCoord.point();
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_correctView();
	}

	void Widget::setPosition(const IntPoint& _pos)
	{
		// обновляем абсолютные координаты
		mAbsolutePosition += _pos - mCoord.point();
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();

		mCoord = _pos;
		_updateView();

	}

	void Widget::setPosition(const IntCoord& _coord)
	{
		// обновляем абсолютные координаты
		mAbsolutePosition += _coord.point() - mCoord.point();
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();

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
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_setAlign(old, mIsMargin || margin);

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
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_setAlign(old, mIsMargin || margin);

		// запоминаем текущее состояние
		mIsMargin = margin;

	}

	void Widget::_attachToLayerItemKeeper(LayerItemKeeper * _item)
	{
		MYGUI_DEBUG_ASSERT(null != _item, "attached item must be valid");

		// сохраняем, чтобы последующие дети могли приаттачиться
		setLayerItemKeeper(_item);

		RenderItem * renderItem = null;

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			// создаем только если есть хоть один не текстовой сабскин
			if ((null == renderItem) && (false == (*skin)->_isText())) {
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
		// мы уже отаттачены
		if (null == getLayerItemKeeper()) return;

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) {
			(*widget)->_detachFromLayerItemKeeper();
		}

		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			(*skin)->_destroyDrawItem();
		}

		// очищаем 
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

		// если мы приаттаченны, то детачим себя, меняем текстуру, и снова аттачим
		LayerItemKeeper * save = getLayerItemKeeper();
		if (null != save) {
			// позже сделать детач без текста
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

		// просто обновляем
		for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) {
			(*skin)->_updateView();
		}
	}

	void Widget::show()
	{
		if (mShow) return;
		mShow = true;

		if (mInheritedShow) {
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_inheritedShow();
			for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->show();
		}

	}

	void Widget::hide()
	{
		if (false == mShow) return;
		mShow = false;

		// если мы уже скрыты отцом, то рассылать не нужно
		if (mInheritedShow) {
			for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->hide();
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_inheritedHide();
		}

	}

	void Widget::_inheritedShow()
	{
		if (mInheritedShow) return;
		mInheritedShow = true;

		if (mShow) {
			for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->show();
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_inheritedShow();
		}

	}

	void Widget::_inheritedHide()
	{
		if (false == mInheritedShow) return;
		mInheritedShow = false;

		if (mShow) {
			for (VectorCroppedRectanglePtr::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->hide();
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_inheritedHide();
		}

	}

	// дает приоритет виджету при пиккинге
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

	void Widget::_getDragItemInfo(WidgetPtr & _list, size_t & _index)
	{
		_list = mNeedDragDrop ? this : null;
		_index = ITEM_NONE;
		_requestGetDragItemInfo(this, _list, _index);
	}

	void Widget::_setDragItemInfo(size_t _index, bool _set, bool _accept)
	{
	}

	void * Widget::_getDropItemData(size_t _index)
	{
		return null;
	}

	void Widget::setToolTipEnable(bool _enable)
	{
		if (m_toolTipEnable == _enable) return;
		m_toolTipEnable = _enable;

		if (m_toolTipEnable) {
			Gui::getInstance().addFrameListener(this);
			m_toolTipCurrentTime = 0;
		}
		else {
			Gui::getInstance().removeFrameListener(this);
		}
	}

	void Widget::_frameEntered(float _frame)
	{
		IntPoint point = InputManager::getInstance().getMousePosition();
		static IntPoint old_point = point;

		if (old_point != point) {
			if (m_toolTipVisible) {
				m_toolTipVisible = false;
				eventToolTip(this, ToolTipInfo(TOOLTIP_HIDE, ITEM_NONE, IntPoint()));
			}
			m_toolTipCurrentTime = 0;
			old_point = point;
		}
		else {
			bool inside = getAbsoluteRect().inside(point);

			if (inside) {
				m_toolTipCurrentTime += _frame;
				if ( !m_toolTipVisible && m_toolTipCurrentTime > WIDGET_TOOLTIP_TIMEOUT) {

					// проверяем не перекрывают ли нас
					WidgetPtr widget = InputManager::getInstance().getMouseFocusWidget();
					while (widget != 0) {
						if (widget->getName() == mName) {
							m_toolTipVisible = true;
							eventToolTip(this, ToolTipInfo(TOOLTIP_SHOW, _getToolTipIndex(point), point));
							break;
						}
						widget = widget->getParent();
					}
				}
			}
			else {
				if (m_toolTipVisible) {
					m_toolTipVisible = false;
					eventToolTip(this, ToolTipInfo(TOOLTIP_HIDE, ITEM_NONE, IntPoint()));
				}
			}
		}
	}

} // namespace MyGUI
