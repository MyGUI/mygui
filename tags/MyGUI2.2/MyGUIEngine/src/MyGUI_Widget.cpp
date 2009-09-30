/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
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
#include "MyGUI_StaticText.h"

namespace MyGUI
{

	const float WIDGET_TOOLTIP_TIMEOUT = 0.5f;

	Widget::Widget(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		ICroppedRectangle(IntCoord(_coord.point(), _info->getSize()), _align, _croppedParent), // размер по скину
		LayerItem(),
		UserData(),
		mStateInfo(_info->getStateInfo()),
		mMaskPickInfo(_info->getMask()),
		mText(nullptr),
		mMainSkin(nullptr),
		mEnabled(true),
		mInheritsEnabled(true),
		mSubSkinsVisible(true),
		mInheritsVisible(true),
		mAlpha(ALPHA_MIN),
		mInheritsAlpha(true),
		mName(_name),
		mTexture(_info->getTextureName()),
		mParent(_parent),
		mIWidgetCreator(_creator),
		mNeedKeyFocus(false),
		mNeedMouseFocus(true),
		mInheritsPick(false),
		mWidgetClient(nullptr),
		mNeedToolTip(false),
		mEnableToolTip(true),
		mToolTipVisible(false),
		mToolTipCurrentTime(0),
		mToolTipOldIndex(ITEM_NONE),
		mWidgetStyle(_style)
	{

#if MYGUI_DEBUG_MODE == 1
		// проверяем соответсвие входных данных
		if (mWidgetStyle == WidgetStyle::Child)
		{
			MYGUI_ASSERT(mCroppedParent, "must be cropped");
			MYGUI_ASSERT(mParent, "must be parent");
		}
		else if (mWidgetStyle == WidgetStyle::Overlapped)
		{
			MYGUI_ASSERT((mParent == nullptr) == (mCroppedParent == nullptr), "error cropped");
		}
		else if (mWidgetStyle == WidgetStyle::Popup)
		{
			MYGUI_ASSERT(!mCroppedParent, "cropped must be nullptr");
			MYGUI_ASSERT(mParent, "must be parent");
		}
#endif

		// корректируем абсолютные координаты
		mAbsolutePosition = _coord.point();
		if (nullptr != mCroppedParent) mAbsolutePosition += mCroppedParent->getAbsolutePosition();

		// имя отсылателя сообщений
		mWidgetEventSender = this;

		initialiseWidgetSkin(_info, _coord.size());

		// дочернее окно обыкновенное
		if (mWidgetStyle == WidgetStyle::Child)
		{
			// если есть леер, то атачимся
			LayerItemKeeper * layer_item = mParent->getLayerItemKeeper();
			if (layer_item) _attachToLayerItemKeeper(layer_item);
		}
		// дочернее нуно перекрывающееся
		else if (mWidgetStyle == WidgetStyle::Overlapped)
		{
			// дочернее перекрывающееся
			if (mParent)
			{
				LayerItemKeeper * layer_item = mParent->getLayerItemKeeper();
				if (layer_item)
				{
					LayerItemKeeper * child = layer_item->createItem();
					_attachToLayerItemKeeper(child);
				}
			}
		}

	}

	Widget::~Widget()
	{
		Gui::getInstance().eventFrameStart -= newDelegate(this, &Widget::frameEntered);

		if (mToolTipVisible) eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));

		shutdownWidgetSkin(true);

		_destroyAllChildWidget();
	}

	void Widget::changeWidgetSkin(const std::string& _skinname)
	{
		WidgetSkinInfoPtr skin_info = SkinManager::getInstance().getSkin(_skinname);
		baseChangeWidgetSkin(skin_info);
	}

	void Widget::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// FIXME
		// если есть клиент то удаляются детишки

		// актуально для рутовых
		std::string layername;
		LayerKeeper * layer = getLayerKeeper();
		if (layer)
		{
			layername = layer->getName();
			LayerManager::getInstance().detachFromLayerKeeper(this);
		}

		// актуально для WidgetStyle::Overlapped и WidgetStyle::Child
		LayerItemKeeper * layer_item = getLayerItemKeeper();

		IntSize size = mCoord.size();

		shutdownWidgetSkin();
		initialiseWidgetSkin(_info, size);

		// дочернее окно обыкновенное
		if (mWidgetStyle == WidgetStyle::Child)
		{
			// если есть леер, то атачимся
			if (layer_item) _attachToLayerItemKeeper(layer_item);
		}
		// дочернее оуно перекрывающееся
		else if (mWidgetStyle == WidgetStyle::Overlapped)
		{
			// дочернее перекрывающееся
			if (mParent)
			{
				if (layer_item)
				{
					_attachToLayerItemKeeper(layer_item);
				}
			}
			// перекрывающееся рутовое
			else
			{
				if (!layername.empty())
				{
					LayerManager::getInstance().attachToLayerKeeper(layername, this);
				}
			}
		}
		else if (mWidgetStyle == WidgetStyle::Popup)
		{
			if (!layername.empty())
			{
				LayerManager::getInstance().attachToLayerKeeper(layername, this);
			}
		}

	}

	void Widget::initialiseWidgetSkin(WidgetSkinInfoPtr _info, const IntSize& _size)
	{
		mTexture = _info->getTextureName();
		mStateInfo = _info->getStateInfo();
		setSize(_info->getSize());

		// загружаем кирпичики виджета
		SubWidgetManager & manager = SubWidgetManager::getInstance();
		for (VectorSubWidgetInfo::const_iterator iter=_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); ++iter)
		{
			ISubWidget * sub = manager.createSubWidget(*iter, this);
			mSubSkinChild.push_back(sub);

			// ищем дефолтные сабвиджеты
			if (mMainSkin == nullptr) mMainSkin = sub->castType<ISubWidgetRect>(false);
			if (mText == nullptr) mText = sub->castType<ISubWidgetText>(false);
		}

		if (false == isRootWidget())
		{
			// проверяем наследуемую скрытость
			if ((!mParent->isVisible()) || (!mParent->_isInheritsVisible()))
			{
				bool value = false;
				mInheritsVisible = value;
				for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin)
					(*skin)->setVisible(value);
				for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
					(*widget)->_setInheritsVisible(value);
				for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
					(*widget)->_setInheritsVisible(value);
			}
			// проверяем наследуемый дизейбл
			if ((!mParent->isEnabled()) || (!mParent->_isInheritsEnable()))
			{
				bool value = false;
				mInheritsEnabled = false;
				for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
					(*iter)->_setInheritsEnable(value);
				for (VectorWidgetPtr::iterator iter = mWidgetChildSkin.begin(); iter != mWidgetChildSkin.end(); ++iter)
					(*iter)->_setInheritsEnable(value);
			}
		}

		setState("normal");

		// парсим свойства
		const MapString & properties = _info->getProperties();
		if (false == properties.empty())
		{
			MapString::const_iterator iter = properties.end();
			if ((iter = properties.find("NeedKey")) != properties.end()) setNeedKeyFocus(utility::parseBool(iter->second));
			if ((iter = properties.find("NeedMouse")) != properties.end()) setNeedMouseFocus(utility::parseBool(iter->second));
			if ((iter = properties.find("Pointer")) != properties.end()) mPointer = iter->second;
			if ((iter = properties.find("Visible")) != properties.end()) setVisible(utility::parseBool(iter->second));

			// OBSOLETE
			if ((iter = properties.find("AlignText")) != properties.end()) _setTextAlign(Align::parse(iter->second));
			if ((iter = properties.find("Colour")) != properties.end()) _setTextColour(Colour::parse(iter->second));
			if ((iter = properties.find("Show")) != properties.end()) setVisible(utility::parseBool(iter->second));
			if ((iter = properties.find("TextAlign")) != properties.end()) _setTextAlign(Align::parse(iter->second));
			if ((iter = properties.find("TextColour")) != properties.end()) _setTextColour(Colour::parse(iter->second));
			if ((iter = properties.find("FontName")) != properties.end()) _setFontName(iter->second);
			if ((iter = properties.find("FontHeight")) != properties.end()) _setFontHeight(utility::parseUInt(iter->second));
		}

		// выставляем альфу, корректировка по отцу автоматически
		setAlpha(ALPHA_MAX);

		// создаем детей скина
		const VectorChildSkinInfo& child = _info->getChild();
		for (VectorChildSkinInfo::const_iterator iter=child.begin(); iter!=child.end(); ++iter)
		{
			WidgetPtr widget = createWidgetT(iter->style, iter->type, iter->skin, iter->coord, iter->align, iter->layer);
			widget->_setInternalData(iter->name);
			// заполняем UserString пропертями
			for (MapString::const_iterator prop=iter->params.begin(); prop!=iter->params.end(); ++prop)
			{
				widget->setUserString(prop->first, prop->second);
			}
			// для детей скина свой список
			mWidgetChildSkin.push_back(widget);
			mWidgetChild.pop_back();
		}

		setSize(_size);
	}

	void Widget::shutdownWidgetSkin(bool _deep)
	{
		// позже сделать детач без текста
		_detachFromLayerItemKeeper(_deep);

		// удаляем все сабскины
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin)
		{
			delete (*skin);
		}
		mSubSkinChild.clear();
		mMainSkin = nullptr;
		mText = nullptr;
		mStateInfo.clear();

		// удаляем виджеты чтобы ли в скине
		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter)
		{
			// Добавляем себя чтобы удалилось
			mWidgetChild.push_back(*iter);
			_destroyChildWidget(*iter);
		}
		mWidgetChildSkin.clear();
	}

	WidgetPtr Widget::baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_style, _type, _skin, _coord, _align, this,
			_style == WidgetStyle::Popup ? nullptr : this, this, _name);

		mWidgetChild.push_back(widget);
		// присоединяем виджет с уровню
		if (!_layer.empty()) LayerManager::getInstance().attachToLayerKeeper(_layer, widget);
		return widget;
	}

	WidgetPtr Widget::createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _name)
	{
		return createWidgetT(_type, _skin, WidgetManager::getInstance().convertRelativeToInt(_coord, this), _align, _name);
	}

	void Widget::_setAlign(const IntCoord& _coord, bool _update)
	{
		// для виджета изменение х у  не меняються
		_setAlign(_coord.size(), _update);
	}

	void Widget::_setAlign(const IntSize& _size, bool _update)
	{
		if (!mCroppedParent) return;

		bool need_move = false;
		bool need_size = false;
		IntCoord coord = mCoord;

		// первоначальное выравнивание
		if (mAlign.isHStretch())
		{
			// растягиваем
			coord.width = mCoord.width + (mCroppedParent->getWidth() - _size.width);
			need_size = true;
		}
		else if (mAlign.isRight())
		{
			// двигаем по правому краю
			coord.left = mCoord.left + (mCroppedParent->getWidth() - _size.width);
			need_move = true;
		}
		else if (mAlign.isHCenter())
		{
			// выравнивание по горизонтали без растяжения
			coord.left = (mCroppedParent->getWidth() - mCoord.width) / 2;
			need_move = true;
		}

		if (mAlign.isVStretch())
		{
			// растягиваем
			coord.height = mCoord.height + (mCroppedParent->getHeight() - _size.height);
			need_size = true;
		}
		else if (mAlign.isBottom())
		{
			// двигаем по нижнему краю
			coord.top = mCoord.top + (mCroppedParent->getHeight() - _size.height);
			need_move = true;
		}
		else if (mAlign.isVCenter())
		{
			// выравнивание по вертикали без растяжения
			coord.top = (mCroppedParent->getHeight() - mCoord.height) / 2;
			need_move = true;
		}

		if (need_move)
		{
			if (need_size) setCoord(coord);
			else setPosition(coord.point());
		}
		else if (need_size)
		{
			setSize(coord.size());
		}
		else _updateView(); // только если не вызвано передвижение и сайз

	}

	void Widget::_updateView()
	{

		bool margin = mCroppedParent ? _checkMargin() : false;

		// вьюпорт стал битым
		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside())
			{
				// запоминаем текущее состояние
				mIsMargin = margin;

				// скрываем
				_setSubSkinVisible(false);

				// для тех кому нужно подправить себя при движении
				//for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

				// вся иерархия должна быть проверенна
				for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateView();
				for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateView();

				return;
			}

		}
		// мы не обрезаны и были нормальные
		else if (false == mIsMargin)
		{
			// запоминаем текущее состояние
			//mIsMargin = margin;

			//_setSubSkinVisible(true);
			// для тех кому нужно подправить себя при движении
			for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

			return;
		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		// если скин был скрыт, то покажем
		_setSubSkinVisible(true);

		// обновляем наших детей, а они уже решат обновлять ли своих детей
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateView();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateView();
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_updateView();

	}

	void Widget::setCaption(const Ogre::UTFString & _caption)
	{
		if (nullptr != mText) mText->setCaption(_caption);
	}

	const Ogre::UTFString & Widget::getCaption()
	{
		if (nullptr == mText)
		{
			static Ogre::UTFString empty;
			return empty;
		}
		return mText->getCaption();
	}

	bool Widget::setState(const std::string & _state)
	{
		MapWidgetStateInfo::const_iterator iter = mStateInfo.find(_state);
		if (iter == mStateInfo.end()) return false;
		size_t index=0;
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin, ++index)
		{
			ISubWidget * info = (*skin);
			StateInfo * data = (*iter).second[index];
			if (data != nullptr) info->_setStateData(data);
		}
		return true;
	}

	void Widget::_destroyChildWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end())
		{
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
		else
		{
			MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
		}
	}

	// удаляет всех детей
	void Widget::_destroyAllChildWidget()
	{
		WidgetManager & manager = WidgetManager::getInstance();
		while (false == mWidgetChild.empty())
		{
			// сразу себя отписывем, иначе вложенной удаление убивает все
			WidgetPtr widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			//if (widget->isRootWidget()) widget->detachWidget();

			// отписываем от всех
			manager.unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			delete widget;
		}
	}

	IntCoord Widget::getClientCoord()
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr) return mWidgetClient->getCoord();
		return IntCoord(0, 0, mCoord.width, mCoord.height);
	}

	void Widget::setAlpha(float _alpha)
	{
		if (mAlpha == _alpha) return;
		mAlpha = _alpha;
		if (nullptr != mParent) mRealAlpha = mAlpha * (mInheritsAlpha ? mParent->_getRealAlpha() : ALPHA_MAX);
		else mRealAlpha = mAlpha;

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAlpha();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateAlpha();
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->setAlpha(mRealAlpha);
	}

	void Widget::_updateAlpha()
	{
		MYGUI_DEBUG_ASSERT(nullptr != mParent, "Widget must have parent");
		mRealAlpha = mAlpha * (mInheritsAlpha ? mParent->_getRealAlpha() : ALPHA_MAX);

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAlpha();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateAlpha();
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->setAlpha(mRealAlpha);
	}

	void Widget::setInheritsAlpha(bool _inherits)
	{
		mInheritsAlpha = _inherits;
		// принудительно обновляем
		float alpha = mAlpha;
		mAlpha = 101;
		setAlpha(alpha);
	}

	LayerItem * Widget::_findLayerItem(int _left, int _top)
	{
		// проверяем попадание
		if (!mSubSkinsVisible
			|| !mEnabled
			|| !mVisible
			|| (!mNeedMouseFocus && !mInheritsPick)
			|| !_checkPoint(_left, _top)
			// если есть маска, проверяем еще и по маске
			|| ((!mMaskPickInfo->empty()) && (!mMaskPickInfo->pick(IntPoint(_left - mCoord.left, _top - mCoord.top), mCoord))))
				return nullptr;
		// спрашиваем у детишек
		for (VectorWidgetPtr::reverse_iterator widget= mWidgetChild.rbegin(); widget != mWidgetChild.rend(); ++widget)
		{
			// общаемся только с послушными детьми
			if ((*widget)->mWidgetStyle == WidgetStyle::Popup) continue;

			LayerItem * item = (*widget)->_findLayerItem(_left - mCoord.left, _top - mCoord.top);
			if (item != nullptr) return item;
		}
		// спрашиваем у детишек скна
		for (VectorWidgetPtr::reverse_iterator widget= mWidgetChildSkin.rbegin(); widget != mWidgetChildSkin.rend(); ++widget)
		{
			LayerItem * item = (*widget)->_findLayerItem(_left - mCoord.left, _top - mCoord.top);
			if (item != nullptr) return item;
		}
		// непослушные дети
		return mInheritsPick ? nullptr : this;
	}

	void Widget::_updateAbsolutePoint()
	{
		// мы рут, нам не надо
		if (!mCroppedParent) return;

		mAbsolutePosition = mCroppedParent->getAbsolutePosition() + mCoord.point();

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateAbsolutePoint();
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_correctView();
	}

	void Widget::setPosition(const IntPoint & _point)
	{
		// обновляем абсолютные координаты
		mAbsolutePosition += _point - mCoord.point();

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateAbsolutePoint();

		mCoord = _point;
		_updateView();
	}

	void Widget::setSize(const IntSize & _size)
	{
		// устанавливаем новую координату а старую пускаем в расчеты
		IntSize old = mCoord.size();
		mCoord = _size;

		bool show = true;

		// обновляем выравнивание
		bool margin = mCroppedParent ? _checkMargin() : false;

		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside())
			{
				// скрываем
				show = false;
			}
		}

		_setSubSkinVisible(show);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_setAlign(old, mIsMargin || margin);

		// запоминаем текущее состояние
		mIsMargin = margin;

	}

	void Widget::setCoord(const IntCoord & _coord)
	{
		// обновляем абсолютные координаты
		mAbsolutePosition += _coord.point() - mCoord.point();

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateAbsolutePoint();

		// устанавливаем новую координату а старую пускаем в расчеты
		IntCoord old = mCoord;
		mCoord = _coord;

		bool show = true;

		// обновляем выравнивание
		bool margin = mCroppedParent ? _checkMargin() : false;

		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside())
			{
				// скрываем
				show = false;
			}
		}

		_setSubSkinVisible(show);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_setAlign(old, mIsMargin || margin);
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_setAlign(old, mIsMargin || margin);

		// запоминаем текущее состояние
		mIsMargin = margin;

	}

	void Widget::_attachToLayerItemKeeper(LayerItemKeeper * _item, bool _deep)
	{
		MYGUI_DEBUG_ASSERT(nullptr != _item, "attached item must be valid");

		// сохраняем, чтобы последующие дети могли приаттачиться
		setLayerItemKeeper(_item);

		RenderItem * renderItem = nullptr;

		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin)
		{
			// создаем только если есть хоть один не текстовой сабскин
			if ((nullptr == renderItem) && (*skin)->firstQueue())
			{
				renderItem = _item->addToRenderItem(mTexture, true, false);
			}
			(*skin)->_createDrawItem(_item, renderItem);
		}

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
		{
			(*widget)->_attachToLayerItemKeeperByStyle(_item, _deep);
		}
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
		{
			(*widget)->_attachToLayerItemKeeperByStyle(_item, _deep);
		}
	}

	void Widget::_attachToLayerItemKeeperByStyle(LayerItemKeeper * _item, bool _deep)
	{
		// разые типа атачим по разному
		WidgetStyle type = mWidgetStyle;

		// всплывающие не трогаем
		if (type == WidgetStyle::Popup)
		{
		}
		// чилды как обычно
		else if (type == WidgetStyle::Child)
		{
			_attachToLayerItemKeeper(_item, _deep);
		}
		// там свои айтемы
		else if (type == WidgetStyle::Overlapped)
		{
			// создаем оверлаппеду новый айтем
			if (_deep) _attachToLayerItemKeeper(_item->createItem(), _deep);
		}
	}

	void Widget::_detachFromLayerItemKeeper(bool _deep)
	{

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
		{
			(*widget)->_detachFromLayerItemKeeperByStyle(_deep);
		}

		// мы уже отаттачены
		LayerItemKeeper * layer_item = getLayerItemKeeper();
		if (layer_item)
		{
			for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_detachFromLayerItemKeeperByStyle(_deep);
			for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin) (*skin)->_destroyDrawItem();

			// при глубокой очистке, если мы оверлаппед, то для нас создавали айтем
			if (_deep && !this->isRootWidget() && mWidgetStyle == WidgetStyle::Overlapped)
			{
				layer_item->destroyItem();
			}
			// очищаем
			setLayerItemKeeper(nullptr);
		}
	}

	void Widget::_detachFromLayerItemKeeperByStyle(bool _deep)
	{
		// разые типа атачим по разному
		WidgetStyle type = mWidgetStyle;

		// всплывающие не трогаем
		if (type == WidgetStyle::Popup)
		{
		}
		// чилды как обычно
		else if (type == WidgetStyle::Child)
		{
			_detachFromLayerItemKeeper(_deep);
		}
		// там свои леер айтемы
		else if (type == WidgetStyle::Overlapped)
		{
			// глубокая очистка
			if (_deep) _detachFromLayerItemKeeper(_deep);
		}
	}

	void Widget::_setUVSet(const FloatRect& _rect)
	{
		if (nullptr != mMainSkin) mMainSkin->_setUVSet(_rect);
	}

	void Widget::_setTextureName(const Ogre::String& _texture)
	{
		if (_texture == mTexture) return;
		mTexture = _texture;

		// если мы приаттаченны, то детачим себя, меняем текстуру, и снова аттачим
		LayerItemKeeper * layer_item = getLayerItemKeeper();
		if (layer_item)
		{
			// позже сделать детач без текста
			_detachFromLayerItemKeeper();
			mTexture = _texture;
			_attachToLayerItemKeeper(layer_item);
		}

	}

	const Ogre::String& Widget::_getTextureName()
	{
		return mTexture;
	}

	void Widget::_setSubSkinVisible(bool _visible)
	{
		if (mSubSkinsVisible == _visible) return;
		mSubSkinsVisible = _visible;

		// просто обновляем
		for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin)
		{
			(*skin)->_updateView();
		}
	}

	void Widget::_forcePeek(WidgetPtr _widget)
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr) mWidgetClient->_forcePeek(_widget);

		size_t size = mWidgetChild.size();
		if ( (size < 2) || (mWidgetChild[size-1] == _widget) ) return;
		for (size_t pos=0; pos<size; pos++)
		{
			if (mWidgetChild[pos] == _widget)
			{
				mWidgetChild[pos] = mWidgetChild[size-1];
				mWidgetChild[size-1] = _widget;
				return;
			}
		}
	}

	const std::string& Widget::getLayerName()
	{
		LayerKeeper * keeper = getLayerKeeper();
		if (nullptr == keeper)
		{
			static std::string empty;
			return empty;
		}
		return keeper->getName();
	}

	void Widget::_getContainer(WidgetPtr & _list, size_t & _index)
	{
		_list = nullptr;
		_index = ITEM_NONE;
		_requestGetContainer(this, _list, _index);
	}

	WidgetPtr Widget::findWidget(const std::string & _name)
	{
		if (_name == mName) return this;
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr) return mWidgetClient->findWidget(_name);
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
		{
			WidgetPtr find = (*widget)->findWidget(_name);
			if (nullptr != find) return find;
		}
		return nullptr;
	}

	void Widget::setNeedToolTip(bool _need)
	{
		if (mNeedToolTip == _need) return;
		mNeedToolTip = _need;

		if (mNeedToolTip)
		{
			Gui::getInstance().eventFrameStart += newDelegate(this, &Widget::frameEntered);
			mToolTipCurrentTime = 0;
		}
		else
		{
			Gui::getInstance().eventFrameStart -= newDelegate(this, &Widget::frameEntered);
		}
	}

	void Widget::frameEntered(float _frame)
	{
		if ( ! mEnableToolTip ) return;

		IntPoint point = InputManager::getInstance().getMousePosition();

		if (mToolTipOldPoint != point)
		{

			mToolTipCurrentTime = 0;

			bool inside = getAbsoluteRect().inside(point);
			if (inside)
			{
				inside = false;
				// проверяем не перекрывают ли нас
				WidgetPtr widget = InputManager::getInstance().getMouseFocusWidget();
				while (widget != 0)
				{
					if (widget/*->getName()*/ == this/*mName*/)
					{
						inside = true;
						break;
					}
					// если виджет берет тултип, значит сбрасываем
					if (widget->getNeedToolTip())
						widget = 0;//widget->getParent();
					else 
						widget = widget->getParent();
				}

				if (inside)
				{
					// теперь смотрим, не поменялся ли индекс внутри окна
					size_t index = _getContainerIndex(point);
					if (mToolTipOldIndex != index)
					{
						if (mToolTipVisible)
						{
							mToolTipCurrentTime = 0;
							mToolTipVisible = false;
							eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
						}
						mToolTipOldIndex = index;
					}

				}
				else
				{
					if (mToolTipVisible)
					{
						mToolTipCurrentTime = 0;
						mToolTipVisible = false;
						eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
					}
				}

			}
			else
			{
				if (mToolTipVisible)
				{
					mToolTipCurrentTime = 0;
					mToolTipVisible = false;
					eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
				}
			}

			mToolTipOldPoint = point;
		}
		else
		{
			bool inside = getAbsoluteRect().inside(point);
			if (inside)
			{
				inside = false;
				// проверяем не перекрывают ли нас
				WidgetPtr widget = InputManager::getInstance().getMouseFocusWidget();
				while (widget != 0)
				{
					if (widget/*->getName()*/ == this/*mName*/)
					{
						inside = true;
						break;
					}
					// если виджет берет тултип, значит сбрасываем
					if (widget->getNeedToolTip())
						widget = 0;//widget->getParent();
					else 
						widget = widget->getParent();
				}

				if (inside)
				{
					if ( ! mToolTipVisible)
					{
						mToolTipCurrentTime += _frame;
						if (mToolTipCurrentTime > WIDGET_TOOLTIP_TIMEOUT)
						{
							mToolTipVisible = true;
							eventToolTip(this, ToolTipInfo(ToolTipInfo::Show, mToolTipOldIndex, point));
						}
					}
				}
			}
		}
	}

	void Widget::setEnableToolTip(bool _enable)
	{
		if (_enable == mEnableToolTip) return;
		mEnableToolTip = _enable;

		if ( ! mEnableToolTip)
		{
			if (mToolTipVisible)
			{
				mToolTipCurrentTime = 0;
				mToolTipVisible = false;
				eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
			}
		}
		else
		{
			mToolTipCurrentTime = 0;
		}
	}

	void Widget::_resetContainer(bool _updateOnly)
	{
		if ( mEnableToolTip)
		{
			if (mToolTipVisible)
			{
				mToolTipVisible = false;
				eventToolTip(this, ToolTipInfo(ToolTipInfo::Hide));
			}
			mToolTipCurrentTime = 0;
			mToolTipOldIndex = ITEM_NONE;
		}
	}

	void Widget::setMaskPick(const std::string & _filename)
	{
		if (mOwnMaskPickInfo.load(_filename))
		{
			mMaskPickInfo = &mOwnMaskPickInfo;
		}
		else
		{
			MYGUI_LOG(Error, "mask not load '" << _filename << "'");
		}
	}

	void Widget::setRealPosition(const FloatPoint & _point)
	{
		const IntCoord & coord = WidgetManager::getInstance().convertRelativeToInt(FloatCoord(_point.left, _point.top, 0, 0), getCroppedParent() == nullptr ? nullptr : mParent);
		setPosition(coord.point());
	}

	void Widget::setRealSize(const FloatSize & _size)
	{
		const IntCoord & coord = WidgetManager::getInstance().convertRelativeToInt(FloatCoord(0, 0, _size.width, _size.height), getCroppedParent() == nullptr ? nullptr : mParent);
		setSize(coord.size());
	}

	void Widget::setRealCoord(const FloatCoord & _coord)
	{
		const IntCoord & coord = WidgetManager::getInstance().convertRelativeToInt(_coord, getCroppedParent() == nullptr ? nullptr : mParent);
		setCoord(coord);
	}

	void Widget::_linkChildWidget(WidgetPtr _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter == mWidgetChild.end(), "widget already exist");
		mWidgetChild.push_back(_widget);
	}

	void Widget::_unlinkChildWidget(WidgetPtr _widget)
	{
		VectorWidgetPtr::iterator iter = std::remove(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter != mWidgetChild.end(), "widget not found");
		mWidgetChild.erase(iter);
	}

	void Widget::detachFromWidget()
	{
		// проверить альфу и видимость

		WidgetPtr root = getParent();
		if (root)
		{
			// отдетачиваемся от лееров
			if (! isRootWidget())
			{
				_detachFromLayerItemKeeper(true);
				mCroppedParent = nullptr;
				mWidgetStyle = WidgetStyle::Overlapped;
			}

			// нам нужен самый рутовый парент
			while (root->getParent())
			{
				root = root->getParent();
			}

			mIWidgetCreator = root->mIWidgetCreator;
			mIWidgetCreator->_linkChildWidget(this);
			mParent->_unlinkChildWidget(this);
			mParent = nullptr;
		}

	}

	void Widget::attachToWidget(WidgetPtr _parent)
	{
		MYGUI_ASSERT(_parent, "parent must be valid");
		MYGUI_ASSERT(_parent != this, "cyclic attach (attaching to self)");

		// attach to client if widget have it
		if (_parent->getClientWidget()) _parent = _parent->getClientWidget();

		// проверяем на цикличность атача
		WidgetPtr parent = _parent;
		while (parent->getParent())
		{
			MYGUI_ASSERT(parent != this, "cyclic attach");
			parent = parent->getParent();
		}


		// отдетачиваемся от лееров
		detachFromWidget();

		mIWidgetCreator->_unlinkChildWidget(this);
		mIWidgetCreator = _parent;
		mParent = _parent;
		mParent->_linkChildWidget(this);

		mWidgetStyle = WidgetStyle::Popup;
		mCroppedParent = nullptr;

	}

	void Widget::setWidgetStyle(WidgetStyle _style)
	{
		if (_style == mWidgetStyle) return;

		// ищем леер к которому мы присоедененны
		WidgetPtr root = this;
		while (!root->isRootWidget())
		{
			root = root->getParent();
		};

		// отсоединяем рут
		std::string layername;
		LayerKeeper * layer = root->getLayerKeeper();
		if (layer)
		{
			layername = layer->getName();
			LayerManager::getInstance().detachFromLayerKeeper(root);

			// если мы рут, то придется отцеплят более высокого рута
			if (root == this)
			{
				layername.clear();

				if (getParent())
				{
					// ищем леер к которому мы присоедененны
					root = getParent();
					while (!root->isRootWidget())
					{
						root = root->getParent();
					};

					layer = root->getLayerKeeper();
					if (layer)
					{
						layername = layer->getName();
						LayerManager::getInstance().detachFromLayerKeeper(root);
					}

				}
			}
		}

		// корректируем
		mWidgetStyle = _style;
		if (_style == WidgetStyle::Child)
		{
			WidgetPtr parent = getParent();
			if (parent)
			{
				mAbsolutePosition = parent->getAbsolutePosition() + mCoord.point();
				mCroppedParent = parent;
				for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();
				for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateAbsolutePoint();
			}

		}
		else if (_style == WidgetStyle::Popup)
		{
			mCroppedParent = nullptr;
			// обновляем координаты
			mAbsolutePosition = mCoord.point();
			// сбрасываем обрезку
			mMargin.clear();

			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();
			for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateAbsolutePoint();

		}
		else if (_style == WidgetStyle::Overlapped)
		{
			WidgetPtr parent = getParent();
			if (parent)
			{
				mAbsolutePosition = parent->getAbsolutePosition() + mCoord.point();
				mCroppedParent = parent;
				for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget) (*widget)->_updateAbsolutePoint();
				for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_updateAbsolutePoint();
			}

		}

		// присоединяем обратно
		if (!layername.empty())
		{
			LayerManager::getInstance().attachToLayerKeeper(layername, root);
		}

	}

	void Widget::_setTextAlign(Align _align)
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) text->setTextAlign(_align);

		if (mText != nullptr) mText->setTextAlign(_align);
	}

	Align Widget::_getTextAlign()
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) return text->getTextAlign();

		if (mText != nullptr) return mText->getTextAlign();
		return Align::Default;
	}

	void Widget::_setTextColour(const Colour& _colour)
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) return text->setTextColour(_colour);

		if (nullptr != mText) mText->setTextColour(_colour);
	}

	const Colour& Widget::_getTextColour()
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) return text->getTextColour();

		return (nullptr == mText) ? Colour::Zero : mText->getTextColour();
	}

	void Widget::_setFontName(const Ogre::String & _font)
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) text->setFontName(_font);

		if (nullptr != mText) mText->setFontName(_font);
	}

	const std::string & Widget::_getFontName()
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) return text->getFontName();

		if (nullptr == mText)
		{
			static std::string empty;
			return empty;
		}
		return mText->getFontName();
	}

	void Widget::_setFontHeight(uint _height)
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) text->setFontHeight(_height);

		if (nullptr != mText) mText->setFontHeight(_height);
	}

	uint Widget::_getFontHeight()
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) return text->getFontHeight();

		return (nullptr == mText) ? 0 : mText->getFontHeight();
	}

	IntSize Widget::_getTextSize()
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) return text->getTextSize();

		return (nullptr == mText) ? IntSize() : mText->getTextSize();
	}

	IntCoord Widget::_getTextRegion()
	{
		StaticTextPtr text = this->castType<StaticText>(false);
		if (text) return text->getTextRegion();

		return (nullptr == mText) ? IntCoord() : mText->getCoord();
	}

	const std::string& Widget::getPointer()
	{
		if (false == mEnabled)
		{
			static std::string empty;
			return empty;
		}
		return mPointer;
	}

	EnumeratorWidgetPtr Widget::getEnumerator()
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr) return mWidgetClient->getEnumerator();
		return Enumerator<VectorWidgetPtr>(mWidgetChild.begin(), mWidgetChild.end());
	}

	size_t Widget::getChildCount()
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr) return mWidgetClient->getChildCount();
		return mWidgetChild.size();
	}

	WidgetPtr Widget::getChildAt(size_t _index)
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr) return mWidgetClient->getChildAt(_index);
		MYGUI_ASSERT_RANGE(_index, mWidgetChild.size(), "Widget::getChildAt");
		return mWidgetChild[_index];
	}

	void Widget::baseUpdateEnable()
	{
		if (mEnabled)
		{
			setState("normal");
		}
		else
		{
			setState("disabled");
		}
	}

	void Widget::setVisible(bool _value)
	{
		if (mVisible == _value) return;
		mVisible = _value;

		if (mInheritsVisible)
		{
			for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin)
				(*skin)->setVisible(_value);
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
				(*widget)->_setInheritsVisible(_value);
			for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
				(*widget)->_setInheritsVisible(_value);
		}

	}

	void Widget::_setInheritsVisible(bool _value)
	{
		if (mInheritsVisible == _value) return;
		mInheritsVisible = _value;

		if (mVisible)
		{
			for (VectorSubWidget::iterator skin = mSubSkinChild.begin(); skin != mSubSkinChild.end(); ++skin)
				(*skin)->setVisible(_value);
			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
				(*widget)->_setInheritsVisible(_value);
			for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
				(*widget)->_setInheritsVisible(_value);
		}
	}

	void Widget::setEnabled(bool _value)
	{
		if (mEnabled == _value) return;
		mEnabled = _value;

		if (mInheritsEnabled)
		{
			for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
				(*iter)->_setInheritsEnable(_value);
			for (VectorWidgetPtr::iterator iter = mWidgetChildSkin.begin(); iter != mWidgetChildSkin.end(); ++iter)
				(*iter)->_setInheritsEnable(_value);

			baseUpdateEnable();
		}

		if (!mEnabled)
		{
			InputManager::getInstance()._unlinkWidget(this);
		}
	}

	void Widget::_setInheritsEnable(bool _value)
	{
		if (mInheritsEnabled == _value) return;
		mInheritsEnabled = _value;

		if (mEnabled)
		{
			for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
				(*iter)->_setInheritsEnable(_value);
			for (VectorWidgetPtr::iterator iter = mWidgetChildSkin.begin(); iter != mWidgetChildSkin.end(); ++iter)
				(*iter)->_setInheritsEnable(_value);

			baseUpdateEnable();
		}

		if (!mEnabled)
		{
			InputManager::getInstance()._unlinkWidget(this);
		}
	}

} // namespace MyGUI

