/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
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
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_ToolTipManager.h"

namespace MyGUI
{

	Widget::Widget() :
		mWidgetClient(nullptr),
		mEnabled(true),
		mInheritsEnabled(true),
		mInheritsVisible(true),
		mAlpha(ALPHA_MAX),
		mRealAlpha(ALPHA_MAX),
		mInheritsAlpha(true),
		mParent(nullptr),
		mWidgetStyle(WidgetStyle::Child),
		mContainer(nullptr),
		mAlign(Align::Default),
		mVisible(true),
		mVisualParent(nullptr),
		mIsMargin(false)
	{
	}

	Widget::~Widget()
	{
	}

	void Widget::_initialise(WidgetStyle _style, ResourceSkin* _info, Widget* _parent)
	{
		mWidgetStyle = _style;

		mVisualParent = mWidgetStyle == WidgetStyle::Popup ? nullptr : _parent;
		mParent = _parent;

		initialiseWidgetSkinBase(_info);


#if MYGUI_DEBUG_MODE == 1
		// проверяем соответсвие входных данных
		if (mWidgetStyle == WidgetStyle::Child)
		{
			MYGUI_ASSERT(mVisualParent, "must be visual");
			MYGUI_ASSERT(mParent, "must be parent");
		}
		else if (mWidgetStyle == WidgetStyle::Overlapped)
		{
			MYGUI_ASSERT((mParent == nullptr) == (mVisualParent == nullptr), "error visual");
		}
		else if (mWidgetStyle == WidgetStyle::Popup)
		{
			MYGUI_ASSERT(!mVisualParent, "visual must be nullptr");
			MYGUI_ASSERT(mParent, "must be parent");
		}
#endif

		attachVisual();

		// витр метод для наследников
		initialiseWidgetSkin(_info);

		// корректируем абсолютные координаты (корректировать при атаче)
		_updateAbsolutePoint();
	}

	void Widget::_shutdown()
	{
		// витр метод для наследников
		shutdownWidgetSkin();

		detachVisual();

		shutdownWidgetSkinBase();

		destroyAllChildWidget();

		mParent = nullptr;
		mVisualParent = nullptr;
	}

	void Widget::initialiseWidgetSkinBase(ResourceSkin* _info)
	{
		//SAVE
		const IntSize& _size = mCoord.size();

		//FIXME - явный вызов
		Widget::setSize(_info->getSize());

		_createSkinItem(_info);

		_updateVisible();
		_updateEnabled();
		_updateAlpha();

		// парсим свойства
		const MapString& properties = _info->getProperties();
		if (!properties.empty())
		{
			MapString::const_iterator iter = properties.end();
			if ((iter = properties.find("NeedKey")) != properties.end()) setNeedKeyFocus(utility::parseValue<bool>(iter->second));
			if ((iter = properties.find("NeedMouse")) != properties.end()) setNeedMouseFocus(utility::parseValue<bool>(iter->second));
			if ((iter = properties.find("Pointer")) != properties.end()) setPointer(iter->second);
			if ((iter = properties.find("Visible")) != properties.end()) setVisible(utility::parseValue<bool>(iter->second));
		}

		// создаем детей скина
		const VectorChildSkinInfo& child = _info->getChild();
		for (VectorChildSkinInfo::const_iterator iter=child.begin(); iter!=child.end(); ++iter)
		{
			Widget* widget = baseCreateWidget(iter->style, iter->type, iter->skin, iter->coord, iter->align, iter->layer, "", true);
			widget->_setInternalData(iter->name);
			// заполняем UserString пропертями
			for (MapString::const_iterator prop=iter->params.begin(); prop!=iter->params.end(); ++prop)
				widget->setUserString(prop->first, prop->second);
		}

		setMaskPick(_info->getMask());

		//FIXME - явный вызов
		Widget::setSize(_size);
	}

	void Widget::shutdownWidgetSkinBase()
	{
		setMaskPick("");

		_deleteSkinItem();

		// удаляем виджеты чтобы ли в скине
		while (!mWidgetChildSkin.empty())
			_destroyChildWidget(mWidgetChildSkin.front(), mWidgetChildSkin);
	}

	Widget* Widget::baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name, bool _template)
	{
		Widget* widget = nullptr;

		if (mWidgetClient != nullptr && !_template)
		{
			widget = mWidgetClient->createWidgetT(_style, _type, _skin, _coord, _align, _layer, _name);
		}
		else
		{
			widget = WidgetManager::getInstance().createWidget(_style, _type, _skin, this);

			if (_template)
				mWidgetChildSkin.push_back(widget);
			else
				mWidgetChild.push_back(widget);

			widget->setAlign(_align);
			widget->setName(_name);
			widget->setCoord(_coord);

			// присоединяем виджет с уровню
			if (!_layer.empty() && widget->_isRootWidget())
				LayerManager::getInstance().attachToLayerNode(_layer, widget);
		}

		_onChildAdded(widget);

		return widget;
	}

	void Widget::_updateView()
	{
		bool margin = mVisualParent ? _checkMargin(mVisualParent) : false;

		// вьюпорт стал битым
		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside(mVisualParent))
			{
				// запоминаем текущее состояние
				mIsMargin = margin;

				// скрываем
				_setSubSkinVisible(false);

				// вся иерархия должна быть проверенна
				for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
					(*widget)->_updateView();
				for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
					(*widget)->_updateView();

				return;
			}

		}
		// мы не обрезаны и были нормальные
		else if (!mIsMargin)
		{
			_updateSkinItemView();
			return;
		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		// если скин был скрыт, то покажем
		_setSubSkinVisible(true);

		// обновляем наших детей, а они уже решат обновлять ли своих детей
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
			(*widget)->_updateView();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
			(*widget)->_updateView();

		_updateSkinItemView();
	}

	void Widget::_destroyChildWidget(Widget* _widget, VectorWidgetPtr& _childs)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(_childs.begin(), _childs.end(), _widget);
		if (iter != _childs.end())
		{
			// удаляем из списка
			_childs.erase(iter);

			// отписываем от всех
			WidgetManager::getInstance().unlinkFromUnlinkers(_widget);

			// непосредственное удаление
			WidgetManager::getInstance()._deleteWidget(_widget);
		}
		else
		{
			MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
		}
	}

	void Widget::destroyChildWidget(Widget* _widget)
	{
		_destroyChildWidget(_widget, mWidgetChild);
	}

	void Widget::destroyAllChildWidget()
	{
		while (!mWidgetChild.empty())
			destroyChildWidget(mWidgetChild.front());
	}

	IntCoord Widget::getClientCoord()
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr)
			return mWidgetClient->getCoord();
		return IntCoord(0, 0, mCoord.width, mCoord.height);
	}

	void Widget::setAlpha(float _alpha)
	{
		if (mAlpha == _alpha)
			return;
		mAlpha = _alpha;

		_updateAlpha();
	}

	void Widget::_updateAlpha()
	{
		if (nullptr != mParent)
			mRealAlpha = mAlpha * (mInheritsAlpha ? mParent->_getRealAlpha() : ALPHA_MAX);
		else
			mRealAlpha = mAlpha;

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
			(*widget)->_updateAlpha();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
			(*widget)->_updateAlpha();

		_setSkinItemAlpha(mRealAlpha);
	}

	void Widget::setInheritsAlpha(bool _inherits)
	{
		mInheritsAlpha = _inherits;

		_updateAlpha();
	}

	ILayerItem * Widget::getLayerItemByPoint(int _left, int _top)
	{
		// проверяем попадание
		if (!mEnabled
			|| !mVisible
			|| (!getNeedMouseFocus() && !getInheritsPick())
			|| !_getViewCoord().inside(IntPoint(_left, _top))
			// если есть маска, проверяем еще и по маске
			|| isMaskPickInside(IntPoint(_left - mCoord.left, _top - mCoord.top), mCoord))
				return nullptr;

		// спрашиваем у детишек
		for (VectorWidgetPtr::reverse_iterator widget= mWidgetChild.rbegin(); widget != mWidgetChild.rend(); ++widget)
		{
			// общаемся только с послушными детьми
			if ((*widget)->mWidgetStyle == WidgetStyle::Popup)
				continue;

			ILayerItem * item = (*widget)->getLayerItemByPoint(_left - mCoord.left, _top - mCoord.top);
			if (item != nullptr)
				return item;
		}
		// спрашиваем у детишек скна
		for (VectorWidgetPtr::reverse_iterator widget= mWidgetChildSkin.rbegin(); widget != mWidgetChildSkin.rend(); ++widget)
		{
			ILayerItem * item = (*widget)->getLayerItemByPoint(_left - mCoord.left, _top - mCoord.top);
			if (item != nullptr)
				return item;
		}

		// непослушные дети
		return getInheritsPick() ? nullptr : this;
	}

	void Widget::_updateAbsolutePoint()
	{
		if (mVisualParent)
			mAbsolutePosition = mVisualParent->getAbsolutePosition() + mCoord.point();
		else
			mAbsolutePosition = mCoord.point();

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
			(*widget)->_updateAbsolutePoint();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
			(*widget)->_updateAbsolutePoint();

		_correctSkinItemView();
	}

	void Widget::_forcePeek(Widget* _widget)
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr)
			mWidgetClient->_forcePeek(_widget);

		VectorWidgetPtr::iterator item = std::remove(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (item != mWidgetChild.end())
		{
			mWidgetChild.erase(item);
			mWidgetChild.insert(mWidgetChild.begin(), _widget);
		}
	}

	Widget* Widget::findWidget(const std::string& _name)
	{
		if (_name == mName)
			return this;

		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr)
			return mWidgetClient->findWidget(_name);

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
		{
			Widget* find = (*widget)->findWidget(_name);
			if (nullptr != find)
				return find;
		}
		return nullptr;
	}

	void Widget::setRealPosition(const FloatPoint& _point)
	{
		setPosition(CoordConverter::convertFromRelative(_point, mVisualParent == nullptr ? RenderManager::getInstance().getViewSize() : mVisualParent->getSize()));
	}

	void Widget::setRealSize(const FloatSize& _size)
	{
		setSize(CoordConverter::convertFromRelative(_size, mVisualParent == nullptr ? RenderManager::getInstance().getViewSize() : mVisualParent->getSize()));
	}

	void Widget::setRealCoord(const FloatCoord& _coord)
	{
		setCoord(CoordConverter::convertFromRelative(_coord, mVisualParent == nullptr ? RenderManager::getInstance().getViewSize() : mVisualParent->getSize()));
	}

	void Widget::_setAlign(const IntSize& _oldsize)
	{
		const IntSize& size = getParentSize();

		bool need_move = false;
		bool need_size = false;
		IntCoord coord = mCoord;

		// первоначальное выравнивание
		if (mAlign.isHStretch())
		{
			// растягиваем
			coord.width = mCoord.width + (size.width - _oldsize.width);
			need_size = true;
		}
		else if (mAlign.isRight())
		{
			// двигаем по правому краю
			coord.left = mCoord.left + (size.width - _oldsize.width);
			need_move = true;
		}
		else if (mAlign.isHCenter())
		{
			// выравнивание по горизонтали без растяжения
			coord.left = (size.width - mCoord.width) / 2;
			need_move = true;
		}

		if (mAlign.isVStretch())
		{
			// растягиваем
			coord.height = mCoord.height + (size.height - _oldsize.height);
			need_size = true;
		}
		else if (mAlign.isBottom())
		{
			// двигаем по нижнему краю
			coord.top = mCoord.top + (size.height - _oldsize.height);
			need_move = true;
		}
		else if (mAlign.isVCenter())
		{
			// выравнивание по вертикали без растяжения
			coord.top = (size.height - mCoord.height) / 2;
			need_move = true;
		}

		if (need_move)
		{
			if (need_size)
				setCoord(coord);
			else
				setPosition(coord.point());
		}
		else if (need_size)
		{
			setSize(coord.size());
		}
		else
		{
			_updateView(); // только если не вызвано передвижение и сайз
		}
	}

	void Widget::setPosition(const IntPoint& _point)
	{
		// обновляем абсолютные координаты
		mAbsolutePosition += _point - mCoord.point();

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
			(*widget)->_updateAbsolutePoint();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
			(*widget)->_updateAbsolutePoint();

		mCoord = _point;

		_updateView();
	}

	void Widget::setSize(const IntSize& _size)
	{
		// устанавливаем новую координату а старую пускаем в расчеты
		IntSize old = mCoord.size();
		mCoord = _size;

		bool visible = true;

		// обновляем выравнивание
		bool margin = mVisualParent ? _checkMargin(mVisualParent) : false;

		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside(mVisualParent))
			{
				// скрываем
				visible = false;
			}
		}

		_setSubSkinVisible(visible);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
			(*widget)->_setAlign(old);
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
			(*widget)->_setAlign(old);

		_setSkinItemAlign(old);

		// запоминаем текущее состояние
		mIsMargin = margin;
	}

	void Widget::setCoord(const IntCoord& _coord)
	{
		// обновляем абсолютные координаты
		mAbsolutePosition += _coord.point() - mCoord.point();

		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
			(*widget)->_updateAbsolutePoint();
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
			(*widget)->_updateAbsolutePoint();

		// устанавливаем новую координату а старую пускаем в расчеты
		IntCoord old = mCoord;
		mCoord = _coord;

		bool visible = true;

		// обновляем выравнивание
		bool margin = mVisualParent ? _checkMargin(mVisualParent) : false;

		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside(mVisualParent))
			{
				// скрываем
				visible = false;
			}
		}

		_setSubSkinVisible(visible);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
			(*widget)->_setAlign(old.size());
		for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
			(*widget)->_setAlign(old.size());

		_setSkinItemAlign(old.size());

		// запоминаем текущее состояние
		mIsMargin = margin;
	}

	void Widget::setAlign(Align _value)
	{
		mAlign = _value;
	}

	Widget* Widget::createWidgetT(const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _name)
	{
		return baseCreateWidget(WidgetStyle::Child, _type, _skin, _coord, _align, "", _name);
	}

	Widget* Widget::createWidgetT(const std::string& _type, const std::string& _skin, int _left, int _top, int _width, int _height, Align _align, const std::string& _name)
	{
		return createWidgetT(_type, _skin, IntCoord(_left, _top, _width, _height), _align, _name);
	}

	Widget* Widget::createWidgetRealT(const std::string& _type, const std::string& _skin, float _left, float _top, float _width, float _height, Align _align, const std::string& _name)
	{
		return createWidgetRealT(_type, _skin, FloatCoord(_left, _top, _width, _height), _align, _name);
	}

	Widget* Widget::createWidgetRealT(const std::string& _type, const std::string& _skin, const FloatCoord& _coord, Align _align, const std::string& _name)
	{
		return createWidgetT(_type, _skin, CoordConverter::convertFromRelative(_coord, getSize()), _align, _name);
	}

	Widget* Widget::createWidgetT(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		return baseCreateWidget(_style, _type, _skin, _coord, _align, _layer, _name);
	}

	EnumeratorWidgetPtr Widget::getEnumerator()
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr)
			return mWidgetClient->getEnumerator();

		return Enumerator<VectorWidgetPtr>(mWidgetChild.begin(), mWidgetChild.end());
	}

	size_t Widget::getChildCount()
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr)
			return mWidgetClient->getChildCount();

		return mWidgetChild.size();
	}

	Widget* Widget::getChildAt(size_t _index)
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		if (mWidgetClient != nullptr)
			return mWidgetClient->getChildAt(_index);

		MYGUI_ASSERT_RANGE(_index, mWidgetChild.size(), "Widget::getChildAt");
		return mWidgetChild[_index];
	}

	void Widget::setProperty(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{Widget, Widget_Position, IntPoint} Sets position
		if (_key == "Widget_Position") setPosition(utility::parseValue<IntPoint>(_value));
		else if (_key == "Widget_Size") setSize(utility::parseValue<IntSize>(_value));
		else if (_key == "Widget_Coord") setCoord(utility::parseValue<IntCoord>(_value));
		else if (_key == "Widget_Visible") setVisible(utility::parseValue<bool>(_value));
		else if (_key == "Widget_Alpha") setAlpha(utility::parseValue<float>(_value));
		else if (_key == "Widget_Colour") setColour(utility::parseValue<Colour>(_value));
		else if (_key == "Widget_InheritsAlpha") setInheritsAlpha(utility::parseValue<bool>(_value));
		else if (_key == "Widget_InheritsPick") setInheritsPick(utility::parseValue<bool>(_value));
		else if (_key == "Widget_MaskPick") setMaskPick(_value);
		else if (_key == "Widget_State") setState(_value);
		else if (_key == "Widget_NeedKey") setNeedKeyFocus(utility::parseValue<bool>(_value));
		else if (_key == "Widget_NeedMouse") setNeedMouseFocus(utility::parseValue<bool>(_value));
		else if (_key == "Widget_Enabled") setEnabled(utility::parseValue<bool>(_value));
		else if (_key == "Widget_NeedToolTip") setNeedToolTip(utility::parseValue<bool>(_value));
		else if (_key == "Widget_Pointer") setPointer(_value);
		else
		{
			MYGUI_LOG(Warning, "Property " << _key << " not found");
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	void Widget::baseUpdateEnable()
	{
		if (mEnabled)
			setState("normal");
		else
			setState("disabled");
	}

	void Widget::setVisible(bool _value)
	{
		if (mVisible == _value)
			return;
		mVisible = _value;

		_updateVisible();
	}

	void Widget::_updateVisible()
	{
		mInheritsVisible = mParent == nullptr || (mParent->getVisible() && mParent->_isInheritsVisible());
		bool value = mVisible && mInheritsVisible;

		_setSkinItemVisible(value);

		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
			(*iter)->_updateVisible();
		for (VectorWidgetPtr::iterator iter = mWidgetChildSkin.begin(); iter != mWidgetChildSkin.end(); ++iter)
			(*iter)->_updateVisible();
	}

	void Widget::setEnabled(bool _value)
	{
		if (mEnabled == _value)
			return;
		mEnabled = _value;

		_updateEnabled();
	}

	void Widget::_updateEnabled()
	{
		mInheritsEnabled = mParent == nullptr || (mParent->getEnabled() && mParent->_isInheritsEnable());
		bool value = mEnabled && mInheritsEnabled;

		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
			(*iter)->_updateEnabled();
		for (VectorWidgetPtr::iterator iter = mWidgetChildSkin.begin(); iter != mWidgetChildSkin.end(); ++iter)
			(*iter)->_updateEnabled();

		baseUpdateEnable();

		if (!value)
			InputManager::getInstance().unlinkWidget(this);
	}

	IntSize Widget::getParentSize()
	{
		if (mVisualParent)
			return static_cast<Widget*>(mVisualParent)->getSize();
		if (getLayer())
			return getLayer()->getSize();

		return RenderManager::getInstance().getViewSize();
	}

	void Widget::_resetContainer(bool _updateOnly)
	{
		if (getNeedToolTip())
			ToolTipManager::getInstance()._unlinkWidget(this);
	}

	void Widget::attachVisual()
	{
		// дочернее окно обыкновенное
		if (mWidgetStyle == WidgetStyle::Child)
		{
			if (mParent)
				mParent->addChildItem(this);
		}
		// дочернее нуно перекрывающееся
		else if (mWidgetStyle == WidgetStyle::Overlapped)
		{
			// дочернее перекрывающееся
			if (mParent)
				mParent->addChildNode(this);
		}
	}

	void Widget::detachVisual()
	{
		// дочернее окно обыкновенное
		if (mWidgetStyle == WidgetStyle::Child)
		{
			if (mParent)
				mParent->removeChildItem(this);
		}
		// дочернее нуно перекрывающееся
		else if (mWidgetStyle == WidgetStyle::Overlapped)
		{
			// дочернее перекрывающееся
			if (mParent)
				mParent->removeChildNode(this);
		}
	}

	/*void Widget::_linkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter == mWidgetChild.end(), "widget already exist");
		mWidgetChild.push_back(_widget);
	}

	void Widget::_unlinkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::remove(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter != mWidgetChild.end(), "widget not found");
		mWidgetChild.erase(iter);
	}*/

	/*void Widget::changeWidgetSkin(const std::string& _skinname)
	{
		ResourceSkin* _info = SkinManager::getInstance().getByName(_skinname);

		shutdownWidgetSkin();

		saveLayerItem();

		shutdownWidgetSkinBase();
		initialiseWidgetSkinBase(_info);

		restoreLayerItem();

		initialiseWidgetSkin(_info);
	}*/

	/*void Widget::detachFromWidget(const std::string& _layer)
	{
		std::string oldlayer = getLayer() != nullptr ? getLayer()->getName() : "";

		Widget* parent = getParent();
		if (parent)
		{
			// отдетачиваемся от лееров
			if ( ! isRootWidget() )
			{
				detachFromLayerItemNode(true);

				if (mWidgetStyle == WidgetStyle::Child)
				{
					mParent->removeChildItem(this);
				}
				else if (mWidgetStyle == WidgetStyle::Overlapped)
				{
					mParent->removeChildNode(this);
				}

				mWidgetStyle = WidgetStyle::Overlapped;

				mVisualParent = nullptr;

				// обновляем координаты
				mAbsolutePosition = mCoord.point();

				for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
					(*widget)->_updateAbsolutePoint();
				for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
					(*widget)->_updateAbsolutePoint();

				// сбрасываем обрезку
				//mMargin.clear();

				_updateView();
			}

			// нам нужен самый рутовый парент
			while (parent->getParent())
				parent = parent->getParent();

			//mIWidgetCreator = parent->mIWidgetCreator;
			//mIWidgetCreator->_linkChildWidget(this);
			Gui::getInstance()._linkChildWidget(this);
			mParent->_unlinkChildWidget(this);
			mParent = nullptr;
		}

		if (!_layer.empty())
		{
			LayerManager::getInstance().attachToLayerNode(_layer, this);
		}
		else if (!oldlayer.empty())
		{
			LayerManager::getInstance().attachToLayerNode(oldlayer, this);
		}

		_updateAlpha();
	}

	void Widget::attachToWidget(Widget* _parent, WidgetStyle _style, const std::string& _layer)
	{
		MYGUI_ASSERT(_parent, "parent must be valid");
		MYGUI_ASSERT(_parent != this, "cyclic attach (attaching to self)");

		// attach to client if widget have it
		if (_parent->getClientWidget())
			_parent = _parent->getClientWidget();

		// проверяем на цикличность атача
		Widget* parent = _parent;
		while (parent->getParent())
		{
			MYGUI_ASSERT(parent != this, "cyclic attach");
			parent = parent->getParent();
		}

		// отдетачиваемся от всего
		detachFromWidget();

		mWidgetStyle = _style;

		if (_style == WidgetStyle::Popup)
		{
			//mIWidgetCreator->_unlinkChildWidget(this);
			//mIWidgetCreator = _parent;
			if (mParent == nullptr)
				Gui::getInstance()._unlinkChildWidget(this);
			else
				mParent->_unlinkChildWidget(this);

			mParent = _parent;
			mParent->_linkChildWidget(this);

			mVisualParent = nullptr;

			if (!_layer.empty())
			{
				LayerManager::getInstance().attachToLayerNode(_layer, this);
			}
		}
		else if (_style == WidgetStyle::Child)
		{
			LayerManager::getInstance().detachFromLayer(this);

			//mIWidgetCreator->_unlinkChildWidget(this);
			//mIWidgetCreator = _parent;
			if (mParent == nullptr)
				Gui::getInstance()._unlinkChildWidget(this);
			else
				mParent->_unlinkChildWidget(this);

			mParent = _parent;
			mParent->_linkChildWidget(this);

			mVisualParent = _parent;
			mAbsolutePosition = _parent->getAbsolutePosition() + mCoord.point();

			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
				(*widget)->_updateAbsolutePoint();
			for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
				(*widget)->_updateAbsolutePoint();

			mParent->addChildItem(this);

			_updateView();
		}
		else if (_style == WidgetStyle::Overlapped)
		{
			LayerManager::getInstance().detachFromLayer(this);

			//mIWidgetCreator->_unlinkChildWidget(this);
			//mIWidgetCreator = _parent;
			if (mParent == nullptr)
				Gui::getInstance()._unlinkChildWidget(this);
			else
				mParent->_unlinkChildWidget(this);

			mParent = _parent;
			mParent->_linkChildWidget(this);

			mVisualParent = _parent;
			mAbsolutePosition = _parent->getAbsolutePosition() + mCoord.point();

			for (VectorWidgetPtr::iterator widget = mWidgetChild.begin(); widget != mWidgetChild.end(); ++widget)
				(*widget)->_updateAbsolutePoint();
			for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget)
				(*widget)->_updateAbsolutePoint();

			mParent->addChildNode(this);

			_updateView();
		}

		_updateAlpha();
	}

	void Widget::setWidgetStyle(WidgetStyle _style, const std::string& _layer)
	{
		if (_style == mWidgetStyle)
			return;
		if (nullptr == getParent())
			return;

		Widget* parent = mParent;

		detachFromWidget();
		attachToWidget(parent, _style, _layer);
	}*/

} // namespace MyGUI
