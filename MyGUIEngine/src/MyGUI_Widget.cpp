/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Gui.h"
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
#include "MyGUI_TextBox.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_ToolTipManager.h"
#include "MyGUI_LayoutManager.h"

namespace MyGUI
{

	void Widget::_initialise(
		WidgetStyle _style,
		const IntCoord& _coord,
		std::string_view _skinName,
		Widget* _parent,
		ICroppedRectangle* _croppedParent,
		std::string_view _name)
	{
		ResourceSkin* skinInfo = nullptr;
		ResourceLayout* templateInfo = nullptr;

		if (LayoutManager::getInstance().isExist(_skinName))
			templateInfo = LayoutManager::getInstance().getByName(_skinName);
		else
			skinInfo = SkinManager::getInstance().getByName(_skinName);

		mCoord = _coord;

		mAlign = Align::Default;
		mWidgetStyle = _style;
		mName = _name;

		mCroppedParent = _croppedParent;
		mParent = _parent;


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

		if (nullptr != mCroppedParent)
			mAbsolutePosition += mCroppedParent->getAbsolutePosition();

		const WidgetInfo* root = initialiseWidgetSkinBase(skinInfo, templateInfo);

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

		// витр метод для наследников
		initialiseOverride();

		if (skinInfo != nullptr)
			setSkinProperty(skinInfo);

		if (root != nullptr)
		{
			for (const auto& property : root->properties)
			{
				setProperty(property.first, property.second);
			}
		}
	}

	void Widget::_shutdown()
	{
		eventWidgetDestroyed(this);

		setUserData(Any::Null);

		// витр метод для наследников
		shutdownOverride();

		shutdownWidgetSkinBase();

		_destroyAllChildWidget();

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

		mParent = nullptr;
		mCroppedParent = nullptr;
	}

	void Widget::changeWidgetSkin(std::string_view _skinName)
	{
		ResourceSkin* skinInfo = nullptr;
		ResourceLayout* templateInfo = nullptr;

		if (LayoutManager::getInstance().isExist(_skinName))
			templateInfo = LayoutManager::getInstance().getByName(_skinName);
		else
			skinInfo = SkinManager::getInstance().getByName(_skinName);

		shutdownOverride();

		saveLayerItem();

		shutdownWidgetSkinBase();
		const WidgetInfo* root = initialiseWidgetSkinBase(skinInfo, templateInfo);

		restoreLayerItem();

		initialiseOverride();

		if (skinInfo != nullptr)
			setSkinProperty(skinInfo);

		if (root != nullptr)
		{
			for (const auto& property : root->properties)
			{
				setProperty(property.first, property.second);
			}
		}
	}

	const WidgetInfo* Widget::initialiseWidgetSkinBase(ResourceSkin* _skinInfo, ResourceLayout* _templateInfo)
	{
		const WidgetInfo* root = nullptr;
		bool skinOnly = false;

		if (_skinInfo == nullptr)
		{
			skinOnly = true;
			std::string_view skinName;

			const VectorWidgetInfo& data = _templateInfo->getLayoutData();
			for (const auto& item : data)
			{
				if (item.name == "Root")
				{
					skinName = item.skin;
					root = &item;
					break;
				}
			}

			_skinInfo = SkinManager::getInstance().getByName(skinName);
		}

		//SAVE
		const IntSize& _size = mCoord.size();

		if (_skinInfo != nullptr)
		{
			//FIXME - явный вызов
			Widget::setSize(_skinInfo->getSize());

			_createSkinItem(_skinInfo);
		}

		// выставляем альфу, корректировка по отцу автоматически
		_updateAlpha();
		_updateEnabled();
		_updateVisible();

		if (!skinOnly)
		{
#ifndef MYGUI_DONT_USE_OBSOLETE
			const MapString& properties = _skinInfo->getProperties();
			for (const auto& property : properties)
			{
				if (BackwardCompatibility::isIgnoreProperty(property.first))
				{
					MYGUI_LOG(
						Warning,
						property.first
							<< " skin property is deprecated, move it to UserString in a widget in ResourceLayout"
							<< LayoutManager::getInstance().getCurrentLayout() << "]");
					setUserString(property.first, property.second);
				}
			}
#endif

			// create skin childs
			const VectorChildSkinInfo& child = _skinInfo->getChild();
			for (const auto& iter : child)
			{
				Widget* widget = baseCreateWidget(
					iter.style,
					iter.type,
					iter.skin,
					iter.coord,
					iter.align,
					iter.layer,
					iter.name,
					true);
				// fill UserString with skin properties
				for (const auto& prop : iter.params)
					widget->setUserString(prop.first, prop.second);
			}
		}

		if (root != nullptr)
		{
			//FIXME - явный вызов
			Widget::setSize(root->intCoord.size());

			for (const auto& userString : root->userStrings)
			{
				setUserString(userString.first, userString.second);
			}

			for (const auto& iter : root->childWidgetsInfo)
			{
				_templateInfo->createWidget(iter, {}, this, true);
			}
		}

		//FIXME - явный вызов
		Widget::setSize(_size);

		return root;
	}

	void Widget::shutdownWidgetSkinBase()
	{
		setMaskPick(MaskPickInfo{});

		_deleteSkinItem();

		// удаляем виджеты чтобы ли в скине
		for (auto& iter : mWidgetChildSkin)
		{
			// Добавляем себя чтобы удалилось
			mWidgetChild.push_back(iter);
			_destroyChildWidget(iter);
		}
		mWidgetChildSkin.clear();

		mWidgetClient = nullptr;
	}

	Widget* Widget::baseCreateWidget(
		WidgetStyle _style,
		std::string_view _type,
		std::string_view _skin,
		const IntCoord& _coord,
		Align _align,
		std::string_view _layer,
		std::string_view _name,
		bool _template)
	{
		Widget* widget = nullptr;

		if (_template)
		{
			widget = WidgetManager::getInstance().createWidget(
				_style,
				_type,
				_skin,
				_coord,
				this,
				_style == WidgetStyle::Popup ? nullptr : this,
				_name);
			mWidgetChildSkin.push_back(widget);
		}
		else
		{
			if (mWidgetClient != nullptr)
			{
				widget =
					mWidgetClient->baseCreateWidget(_style, _type, _skin, _coord, _align, _layer, _name, _template);
				onWidgetCreated(widget);
				return widget;
			}

			widget = WidgetManager::getInstance().createWidget(
				_style,
				_type,
				_skin,
				_coord,
				this,
				_style == WidgetStyle::Popup ? nullptr : this,
				_name);
			addWidget(widget);
		}

		widget->setAlign(_align);

		// присоединяем виджет с уровню
		if (!_layer.empty() && widget->isRootWidget())
			LayerManager::getInstance().attachToLayerNode(_layer, widget);

		onWidgetCreated(widget);

		return widget;
	}

	Widget* Widget::createWidgetRealT(
		std::string_view _type,
		std::string_view _skin,
		const FloatCoord& _coord,
		Align _align,
		std::string_view _name)
	{
		return createWidgetT(_type, _skin, CoordConverter::convertFromRelative(_coord, getSize()), _align, _name);
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

				// вся иерархия должна быть проверенна
				for (auto& widget : mWidgetChild)
					widget->_updateView();
				for (auto& widget : mWidgetChildSkin)
					widget->_updateView();

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
		for (auto& widget : mWidgetChild)
			widget->_updateView();
		for (auto& widget : mWidgetChildSkin)
			widget->_updateView();

		_updateSkinItemView();
	}

	bool Widget::_setWidgetState(std::string_view _state)
	{
		return _setSkinItemState(_state);
	}

	void Widget::_destroyChildWidget(Widget* _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");

		if (mParent != nullptr && mParent->getClientWidget() == this)
			mParent->onWidgetDestroy(_widget);

		onWidgetDestroy(_widget);

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end())
		{
			// сохраняем указатель
			MyGUI::Widget* widget = *iter;

			// удаляем из списка
			mWidgetChild.erase(iter);

			// отписываем от всех
			WidgetManager::getInstance().unlinkFromUnlinkers(_widget);

			// непосредственное удаление
			WidgetManager::getInstance()._deleteWidget(widget);
		}
		else
		{
			MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
		}
	}

	// удаляет всех детей
	void Widget::_destroyAllChildWidget()
	{
		WidgetManager& manager = WidgetManager::getInstance();
		while (!mWidgetChild.empty())
		{
			// сразу себя отписывем, иначе вложенной удаление убивает все
			Widget* widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			manager.unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			WidgetManager::getInstance()._deleteWidget(widget);
		}
	}

	IntCoord Widget::getClientCoord() const
	{
		if (mWidgetClient != nullptr)
			return mWidgetClient->getCoord();
		return {0, 0, mCoord.width, mCoord.height};
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

		for (auto& widget : mWidgetChild)
			widget->_updateAlpha();
		for (auto& widget : mWidgetChildSkin)
			widget->_updateAlpha();

		_setSkinItemAlpha(mRealAlpha);
	}

	void Widget::setInheritsAlpha(bool _inherits)
	{
		mInheritsAlpha = _inherits;
		_updateAlpha();
	}

	ILayerItem* Widget::getLayerItemByPoint(int _left, int _top) const
	{
		// проверяем попадание
		if (!mEnabled || !mInheritedVisible || (!getNeedMouseFocus() && !getInheritsPick()) ||
			!_checkPoint(_left, _top)
			// если есть маска, проверяем еще и по маске
			|| !isMaskPickInside(IntPoint(_left - mCoord.left, _top - mCoord.top), mCoord))
			return nullptr;

		// спрашиваем у детишек
		for (VectorWidgetPtr::const_reverse_iterator widget = mWidgetChild.rbegin(); widget != mWidgetChild.rend();
			 ++widget)
		{
			// общаемся только с послушными детьми
			if ((*widget)->mWidgetStyle == WidgetStyle::Popup)
				continue;

			ILayerItem* item = (*widget)->getLayerItemByPoint(_left - mCoord.left, _top - mCoord.top);
			if (item != nullptr)
				return item;
		}
		// спрашиваем у детишек скина
		for (VectorWidgetPtr::const_reverse_iterator widget = mWidgetChildSkin.rbegin();
			 widget != mWidgetChildSkin.rend();
			 ++widget)
		{
			ILayerItem* item = (*widget)->getLayerItemByPoint(_left - mCoord.left, _top - mCoord.top);
			if (item != nullptr)
				return item;
		}

		// непослушные дети
		return getInheritsPick() ? nullptr : const_cast<Widget*>(this);
	}

	void Widget::_updateAbsolutePoint()
	{
		// мы рут, нам не надо
		if (!mCroppedParent)
			return;

		mAbsolutePosition = mCroppedParent->getAbsolutePosition() + mCoord.point();

		for (auto& widget : mWidgetChild)
			widget->_updateAbsolutePoint();
		for (auto& widget : mWidgetChildSkin)
			widget->_updateAbsolutePoint();

		_correctSkinItemView();
	}

	void Widget::_forcePick(Widget* _widget)
	{
		if (mWidgetClient != nullptr)
		{
			mWidgetClient->_forcePick(_widget);
			return;
		}

		auto iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter == mWidgetChild.end())
			return;

		for (auto& widget : mWidgetChild)
		{
			if (widget == _widget)
				widget->mDepth = -1;
			else if (widget->getDepth() == -1)
				widget->mDepth = 0;
		}
		// code below is an optimized way to setDepth for multiple widgets
		// without calling slow Windget::_updateChilds multiple times
		std::stable_sort(
			mWidgetChild.begin(),
			mWidgetChild.end(),
			[](Widget* lhs, Widget* rhs) { return lhs->getDepth() > rhs->getDepth(); });
		_updateChilds();
	}

	Widget* Widget::findWidget(std::string_view _name)
	{
		if (_name == mName)
			return this;
		if (mWidgetClient != nullptr)
			return mWidgetClient->findWidget(_name);

		for (const auto& widget : mWidgetChild)
		{
			Widget* find = widget->findWidget(_name);
			if (nullptr != find)
				return find;
		}
		return nullptr;
	}

	void Widget::setRealPosition(const FloatPoint& _point)
	{
		setPosition(CoordConverter::convertFromRelative(
			_point,
			mCroppedParent == nullptr ? RenderManager::getInstance().getViewSize() : mCroppedParent->getSize()));
	}

	void Widget::setRealSize(const FloatSize& _size)
	{
		setSize(CoordConverter::convertFromRelative(
			_size,
			mCroppedParent == nullptr ? RenderManager::getInstance().getViewSize() : mCroppedParent->getSize()));
	}

	void Widget::setRealCoord(const FloatCoord& _coord)
	{
		setCoord(CoordConverter::convertFromRelative(
			_coord,
			mCroppedParent == nullptr ? RenderManager::getInstance().getViewSize() : mCroppedParent->getSize()));
	}

	void Widget::_setAlign(const IntSize& _oldsize, const IntSize& _newSize)
	{
		const IntSize& size = _newSize; //getParentSize();

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

		for (auto& widget : mWidgetChild)
			widget->_updateAbsolutePoint();
		for (auto& widget : mWidgetChildSkin)
			widget->_updateAbsolutePoint();

		mCoord = _point;

		_updateView();

		eventChangeCoord(this);
	}

	void Widget::setSize(const IntSize& _size)
	{
		// устанавливаем новую координату а старую пускаем в расчеты
		IntSize old = mCoord.size();
		mCoord = _size;

		bool visible = true;

		// обновляем выравнивание
		bool margin = mCroppedParent ? _checkMargin() : false;

		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside())
			{
				// скрываем
				visible = false;
			}
		}

		_setSubSkinVisible(visible);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (auto& widget : mWidgetChild)
			widget->_setAlign(old, getSize());
		for (auto& widget : mWidgetChildSkin)
			widget->_setAlign(old, getSize());

		_setSkinItemAlign(old);

		// запоминаем текущее состояние
		mIsMargin = margin;

		eventChangeCoord(this);
	}

	void Widget::setCoord(const IntCoord& _coord)
	{
		// обновляем абсолютные координаты
		mAbsolutePosition += _coord.point() - mCoord.point();

		for (auto& widget : mWidgetChild)
			widget->_updateAbsolutePoint();
		for (auto& widget : mWidgetChildSkin)
			widget->_updateAbsolutePoint();

		// устанавливаем новую координату а старую пускаем в расчеты
		IntCoord old = mCoord;
		mCoord = _coord;

		bool visible = true;

		// обновляем выравнивание
		bool margin = mCroppedParent ? _checkMargin() : false;

		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside())
			{
				// скрываем
				visible = false;
			}
		}

		_setSubSkinVisible(visible);

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (auto& widget : mWidgetChild)
			widget->_setAlign(old.size(), getSize());
		for (auto& widget : mWidgetChildSkin)
			widget->_setAlign(old.size(), getSize());

		_setSkinItemAlign(old.size());

		// запоминаем текущее состояние
		mIsMargin = margin;

		eventChangeCoord(this);
	}

	void Widget::setAlign(Align _value)
	{
		mAlign = _value;
	}

	void Widget::detachFromWidget(std::string_view _layer)
	{
		std::string_view oldlayer;
		if (getLayer())
			oldlayer = getLayer()->getName();

		Widget* parent = getParent();
		if (parent)
		{
			// отдетачиваемся от лееров
			if (!isRootWidget())
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

				mCroppedParent = nullptr;

				// обновляем координаты
				mAbsolutePosition = mCoord.point();

				for (auto& widget : mWidgetChild)
					widget->_updateAbsolutePoint();
				for (auto& widget : mWidgetChildSkin)
					widget->_updateAbsolutePoint();

				// сбрасываем обрезку
				mMargin.clear();

				_updateView();
			}

			// нам нужен самый рутовый парент
			while (parent->getParent())
				parent = parent->getParent();

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

	void Widget::attachToWidget(Widget* _parent, WidgetStyle _style, std::string_view _layer)
	{
		MYGUI_ASSERT(_parent, "parent must be valid");
		MYGUI_ASSERT(_parent != this, "cyclic attach (attaching to self)");

		// attach to client if widget have it
		if (_parent->getClientWidget())
			_parent = _parent->getClientWidget();

		Widget* parent = _parent;
		while (parent->getParent())
		{
			MYGUI_ASSERT(parent != this, "cyclic attach");
			parent = parent->getParent();
		}

		detachFromWidget();

		mWidgetStyle = _style;

		if (_style == WidgetStyle::Popup)
		{
			if (mParent == nullptr)
				Gui::getInstance()._unlinkChildWidget(this);
			else
				mParent->_unlinkChildWidget(this);

			mParent = _parent;
			mParent->_linkChildWidget(this);

			mCroppedParent = nullptr;

			if (!_layer.empty())
			{
				LayerManager::getInstance().attachToLayerNode(_layer, this);
			}
		}
		else if (_style == WidgetStyle::Child)
		{
			LayerManager::getInstance().detachFromLayer(this);

			if (mParent == nullptr)
				Gui::getInstance()._unlinkChildWidget(this);
			else
				mParent->_unlinkChildWidget(this);

			mParent = _parent;
			mParent->addChildItem(this);
			mParent->_linkChildWidget(this);

			mCroppedParent = _parent;
			mAbsolutePosition = _parent->getAbsolutePosition() + mCoord.point();

			for (auto& widget : mWidgetChild)
				widget->_updateAbsolutePoint();
			for (auto& widget : mWidgetChildSkin)
				widget->_updateAbsolutePoint();

			_updateView();
		}
		else if (_style == WidgetStyle::Overlapped)
		{
			LayerManager::getInstance().detachFromLayer(this);

			if (mParent == nullptr)
				Gui::getInstance()._unlinkChildWidget(this);
			else
				mParent->_unlinkChildWidget(this);

			mParent = _parent;
			mParent->_linkChildWidget(this);

			mCroppedParent = _parent;
			mAbsolutePosition = _parent->getAbsolutePosition() + mCoord.point();

			for (auto& widget : mWidgetChild)
				widget->_updateAbsolutePoint();
			for (auto& widget : mWidgetChildSkin)
				widget->_updateAbsolutePoint();

			mParent->addChildNode(this);

			_updateView();
		}

		_updateAlpha();
	}

	void Widget::setWidgetStyle(WidgetStyle _style, std::string_view _layer)
	{
		if (_style == mWidgetStyle)
			return;
		if (nullptr == getParent())
			return;

		Widget* parent = mParent;

		detachFromWidget();
		attachToWidget(parent, _style, _layer);
		// ищем леер к которому мы присоедененны
	}

	Widget* Widget::createWidgetT(
		std::string_view _type,
		std::string_view _skin,
		const IntCoord& _coord,
		Align _align,
		std::string_view _name)
	{
		return baseCreateWidget(WidgetStyle::Child, _type, _skin, _coord, _align, {}, _name, false);
	}

	Widget* Widget::createWidgetT(
		std::string_view _type,
		std::string_view _skin,
		int _left,
		int _top,
		int _width,
		int _height,
		Align _align,
		std::string_view _name)
	{
		return createWidgetT(_type, _skin, IntCoord(_left, _top, _width, _height), _align, _name);
	}

	Widget* Widget::createWidgetRealT(
		std::string_view _type,
		std::string_view _skin,
		float _left,
		float _top,
		float _width,
		float _height,
		Align _align,
		std::string_view _name)
	{
		return createWidgetRealT(_type, _skin, FloatCoord(_left, _top, _width, _height), _align, _name);
	}

	Widget* Widget::createWidgetT(
		WidgetStyle _style,
		std::string_view _type,
		std::string_view _skin,
		const IntCoord& _coord,
		Align _align,
		std::string_view _layer,
		std::string_view _name)
	{
		return baseCreateWidget(_style, _type, _skin, _coord, _align, _layer, _name, false);
	}

	EnumeratorWidgetPtr Widget::getEnumerator() const
	{
		if (mWidgetClient != nullptr)
			return mWidgetClient->getEnumerator();
		return {mWidgetChild.begin(), mWidgetChild.end()};
	}

	size_t Widget::getChildCount() const
	{
		if (mWidgetClient != nullptr)
			return mWidgetClient->getChildCount();
		return mWidgetChild.size();
	}

	Widget* Widget::getChildAt(size_t _index) const
	{
		if (mWidgetClient != nullptr)
			return mWidgetClient->getChildAt(_index);
		MYGUI_ASSERT_RANGE(_index, mWidgetChild.size(), "Widget::getChildAt");
		return mWidgetChild[_index];
	}

	void Widget::baseUpdateEnable()
	{
		if (getInheritedEnabled())
			_setWidgetState("normal");
		else
			_setWidgetState("disabled");
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
		mInheritedVisible = mParent == nullptr || mParent->getInheritedVisible();
		mInheritedVisible = mVisible && mInheritedVisible;

		_setSkinItemVisible(mInheritedVisible);

		for (auto& widget : mWidgetChild)
			widget->_updateVisible();
		for (auto& widget : mWidgetChildSkin)
			widget->_updateVisible();

		if (!mInheritedVisible && InputManager::getInstance().getMouseFocusWidget() == this)
			InputManager::getInstance()._resetMouseFocusWidget();
		if (!mInheritedVisible && InputManager::getInstance().getKeyFocusWidget() == this)
			InputManager::getInstance().resetKeyFocusWidget();
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
		mInheritedEnabled = mParent == nullptr || (mParent->getInheritedEnabled());
		mInheritedEnabled = mInheritedEnabled && mEnabled;

		for (auto& iter : mWidgetChild)
			iter->_updateEnabled();
		for (auto& iter : mWidgetChildSkin)
			iter->_updateEnabled();

		baseUpdateEnable();

		if (!mInheritedEnabled)
			InputManager::getInstance().unlinkWidget(this);
	}

	void Widget::setColour(const Colour& _value)
	{
		_setSkinItemColour(_value);

		for (auto& widget : mWidgetChildSkin)
			widget->setColour(_value);
	}

	IntSize Widget::getParentSize() const
	{
		if (mCroppedParent)
			return static_cast<Widget*>(mCroppedParent)->getSize();
		if (getLayer())
			return getLayer()->getSize();

		return RenderManager::getInstance().getViewSize();
	}

	void Widget::_resetContainer(bool _updateOnly)
	{
		if (getNeedToolTip())
			ToolTipManager::getInstance()._unlinkWidget(this);
	}

	bool Widget::_checkPoint(int _left, int _top) const
	{
		return (_getViewLeft() <= _left) && (_getViewTop() <= _top) && (_getViewRight() >= _left) &&
			(_getViewBottom() >= _top);
	}

	void Widget::_linkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter == mWidgetChild.end(), "widget already exist");
		addWidget(_widget);
	}

	void Widget::_unlinkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::remove(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter != mWidgetChild.end(), "widget not found");
		mWidgetChild.erase(iter);
	}

	void Widget::shutdownOverride()
	{
	}

	void Widget::initialiseOverride()
	{
		///@wskin_child{Widget, Widget, Client} Client area, all child widgets are created inside this area.
		assignWidget(mWidgetClient, "Client");
	}

	void Widget::setSkinProperty(ResourceSkin* _info)
	{
		const MapString& properties = _info->getProperties();
		for (const auto& property : properties)
			setProperty(property.first, property.second);
	}

	void Widget::setProperty(std::string_view _key, std::string_view _value)
	{
		std::string key{_key};
		std::string value{_value};

		if (BackwardCompatibility::checkProperty(this, key, value))
		{
			size_t index = key.find('_');
			if (index != std::string::npos)
			{
				MYGUI_LOG(
					Warning,
					"Widget property '" << key << "' have type prefix - use '" << key.substr(index + 1) << "' instead ["
										<< LayoutManager::getInstance().getCurrentLayout() << "]");
				key = key.substr(index + 1);
			}

			setPropertyOverride(key, value);
		}
	}

	VectorWidgetPtr Widget::getSkinWidgetsByName(std::string_view _name) const
	{
		VectorWidgetPtr result;

		for (const auto& childSkin : mWidgetChildSkin)
			childSkin->findWidgets(_name, result);

		return result;
	}

	void Widget::findWidgets(std::string_view _name, VectorWidgetPtr& _result)
	{
		if (_name == mName)
			_result.push_back(this);

		if (mWidgetClient != nullptr)
		{
			mWidgetClient->findWidgets(_name, _result);
		}
		else
		{
			for (const auto& widget : mWidgetChild)
				widget->findWidgets(_name, _result);
		}
	}

	void Widget::onWidgetCreated(Widget* _widget)
	{
	}

	void Widget::onWidgetDestroy(Widget* _widget)
	{
	}

	void Widget::setWidgetClient(Widget* _widget)
	{
		MYGUI_ASSERT(mWidgetClient != this, "mWidgetClient can not be this widget");
		mWidgetClient = _widget;
	}

	Widget* Widget::_getClientWidget()
	{
		return getClientWidget() == nullptr ? this : getClientWidget();
	}

	const Widget* Widget::_getClientWidget() const
	{
		return getClientWidget() == nullptr ? this : getClientWidget();
	}

	Widget* Widget::_createSkinWidget(
		WidgetStyle _style,
		std::string_view _type,
		std::string_view _skin,
		const IntCoord& _coord,
		Align _align,
		std::string_view _layer,
		std::string_view _name)
	{
		return baseCreateWidget(_style, _type, _skin, _coord, _align, _layer, _name, true);
	}

	void Widget::setPropertyOverride(std::string_view _key, std::string_view _value)
	{
		/// @wproperty{Widget, Position, IntPoint} Set widget position.
		if (_key == "Position")
			setPosition(utility::parseValue<IntPoint>(_value));

		/// @wproperty{Widget, Size, IntSize} Set widget size.
		else if (_key == "Size")
			setSize(utility::parseValue<IntSize>(_value));

		/// @wproperty{Widget, Coord, IntCoord} Set widget coordinates (position and size).
		else if (_key == "Coord")
			setCoord(utility::parseValue<IntCoord>(_value));

		/// @wproperty{Widget, Visible, bool} Show or hide widget.
		else if (_key == "Visible")
			setVisible(utility::parseValue<bool>(_value));

		/// @wproperty{Widget, Depth, int} Child widget rendering depth.
		else if (_key == "Depth")
			setDepth(utility::parseValue<int>(_value));

		/// @wproperty{Widget, Alpha, float} Прозрачность виджета от 0 до 1.
		else if (_key == "Alpha")
			setAlpha(utility::parseValue<float>(_value));

		/// @wproperty{Widget, Colour, Colour} Цвет виджета.
		else if (_key == "Colour")
			setColour(utility::parseValue<Colour>(_value));

		/// @wproperty{Widget, InheritsAlpha, bool} Режим наследования прозрачности.
		else if (_key == "InheritsAlpha")
			setInheritsAlpha(utility::parseValue<bool>(_value));

		/// @wproperty{Widget, InheritsPick, bool} Режим наследования доступности мышью.
		else if (_key == "InheritsPick")
			setInheritsPick(utility::parseValue<bool>(_value));

		/// @wproperty{Widget, MaskPick, string} Имя файла текстуры по которому генерится маска для доступности мышью.
		else if (_key == "MaskPick")
			setMaskPick(std::string{_value});

		/// @wproperty{Widget, NeedKey, bool} Режим доступности виджета для ввода с клавиатуры.
		else if (_key == "NeedKey")
			setNeedKeyFocus(utility::parseValue<bool>(_value));

		/// @wproperty{Widget, NeedMouse, bool} Режим доступности виджета для ввода мышью.
		else if (_key == "NeedMouse")
			setNeedMouseFocus(utility::parseValue<bool>(_value));

		/// @wproperty{Widget, Enabled, bool} Режим доступности виджета.
		else if (_key == "Enabled")
			setEnabled(utility::parseValue<bool>(_value));

		/// @wproperty{Widget, NeedToolTip, bool} Режим поддержки тултипов.
		else if (_key == "NeedToolTip")
			setNeedToolTip(utility::parseValue<bool>(_value));

		/// @wproperty{Widget, Pointer, string} Указатель мыши для этого виджета.
		else if (_key == "Pointer")
			setPointer(_value);

		else
		{
			MYGUI_LOG(
				Warning,
				"Widget '" << getName() << "|" << getTypeName() << "' have unknown property '" << _key << "' "
						   << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	void Widget::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void Widget::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void Widget::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

	void Widget::setRealPosition(float _left, float _top)
	{
		setRealPosition(FloatPoint(_left, _top));
	}

	void Widget::setRealSize(float _width, float _height)
	{
		setRealSize(FloatSize(_width, _height));
	}

	void Widget::setRealCoord(float _left, float _top, float _width, float _height)
	{
		setRealCoord(FloatCoord(_left, _top, _width, _height));
	}

	const std::string& Widget::getName() const
	{
		return mName;
	}

	bool Widget::getVisible() const
	{
		return mVisible;
	}

	Align Widget::getAlign() const
	{
		return mAlign;
	}

	float Widget::getAlpha() const
	{
		return mAlpha;
	}

	bool Widget::getInheritsAlpha() const
	{
		return mInheritsAlpha;
	}

	bool Widget::isRootWidget() const
	{
		return nullptr == mCroppedParent;
	}

	Widget* Widget::getParent() const
	{
		return mParent;
	}

	void Widget::setEnabledSilent(bool _value)
	{
		mEnabled = _value;
	}

	bool Widget::getEnabled() const
	{
		return mEnabled;
	}

	Widget* Widget::getClientWidget()
	{
		return mWidgetClient;
	}

	const Widget* Widget::getClientWidget() const
	{
		return mWidgetClient;
	}

	WidgetStyle Widget::getWidgetStyle() const
	{
		return mWidgetStyle;
	}

	size_t Widget::_getItemIndex(Widget* _item) const
	{
		return ITEM_NONE;
	}

	void Widget::_setContainer(Widget* _value)
	{
		mContainer = _value;
	}

	Widget* Widget::_getContainer() const
	{
		return mContainer;
	}

	size_t Widget::_getContainerIndex(const IntPoint& _point) const
	{
		return ITEM_NONE;
	}

	const IntCoord& Widget::getLayerItemCoord() const
	{
		return mCoord;
	}

	float Widget::_getRealAlpha() const
	{
		return mRealAlpha;
	}

	bool Widget::getInheritedEnabled() const
	{
		return mInheritedEnabled;
	}

	bool Widget::getInheritedVisible() const
	{
		return mInheritedVisible;
	}

	void Widget::resizeLayerItemView(const IntSize& _oldView, const IntSize& _newView)
	{
		_setAlign(_oldView, _newView);
	}

	void Widget::setDepth(int _value)
	{
		if (mDepth == _value)
			return;

		mDepth = _value;

		if (mParent != nullptr)
		{
			mParent->_unlinkChildWidget(this);
			mParent->_linkChildWidget(this);
			mParent->_updateChilds();
		}
	}

	int Widget::getDepth() const
	{
		return mDepth;
	}

	void Widget::addWidget(Widget* _widget)
	{
		// сортировка глубины от большого к меньшему

		int depth = _widget->getDepth();

		for (size_t index = 0; index < mWidgetChild.size(); ++index)
		{
			Widget* widget = mWidgetChild[index];
			if (widget->getDepth() < depth)
			{
				mWidgetChild.insert(mWidgetChild.begin() + index, _widget);
				_updateChilds();
				return;
			}
		}

		mWidgetChild.push_back(_widget);
	}

	void Widget::_updateChilds()
	{
		for (auto& widget : mWidgetChild)
		{
			if (widget->getWidgetStyle() == WidgetStyle::Child)
			{
				widget->detachFromLayerItemNode(false);
				removeChildItem(widget);
			}
		}

		for (auto& widget : mWidgetChild)
		{
			if (widget->getWidgetStyle() == WidgetStyle::Child)
			{
				addChildItem(widget);
				widget->_updateView();
			}
		}
	}

} // namespace MyGUI
