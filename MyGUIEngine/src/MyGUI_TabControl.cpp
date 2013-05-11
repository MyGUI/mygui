/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_TabControl.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Button.h"
#include "MyGUI_TabItem.h"
#include "MyGUI_ResourceSkin.h"

namespace MyGUI
{

	const float TAB_SPEED_FADE_COEF = 5.0f;

	TabControl::TabControl() :
		mOffsetTab(0),
		mButtonShow(true),
		mWidthBar(0),
		mWidgetBar(nullptr),
		mButtonLeft(nullptr),
		mButtonRight(nullptr),
		mButtonDecor(nullptr),
		mEmptyBarWidget(nullptr),
		mItemTemplate(nullptr),
		mStartIndex(0),
		mIndexSelect(ITEM_NONE),
		mButtonDefaultWidth(1),
		mSmoothShow(true),
		mButtonAutoWidth(true),
		mShutdown(false),
		mHeaderPlace(nullptr),
		mControls(nullptr),
		mEmpty(nullptr)
	{
	}

	void TabControl::initialiseOverride()
	{
		Base::initialiseOverride();

		if (isUserString("ButtonSkin"))
			mButtonSkinName = getUserString("ButtonSkin");

		// OBSOLETE
		if (isUserString("OffsetBar"))
			mOffsetTab = utility::parseValue<int>(getUserString("OffsetBar"));

		// OBSOLETE
		if (isUserString("EmptyBarSkin"))
			mEmptySkinName = getUserString("EmptyBarSkin");

		// OBSOLETE
		assignWidget(mWidgetBar, "Bar");
		if (mWidgetBar != nullptr)
		{
			mWidgetBar->setSize(mWidgetBar->getWidth() - mOffsetTab, mWidgetBar->getHeight());
		}

		///@wskin_child{TabControl, Button, Left} Кнопка прокрутки заголовков влево.
		assignWidget(mButtonLeft, "Left");
		if (mButtonLeft != nullptr)
		{
			mButtonLeft->eventMouseButtonClick += newDelegate(this, &TabControl::notifyPressedButtonEvent);
		}

		///@wskin_child{TabControl, Button, Right} Кнопка прокрутки заголовков вправо.
		assignWidget(mButtonRight, "Right");
		if (mButtonRight != nullptr)
		{
			mButtonRight->eventMouseButtonClick += newDelegate(this, &TabControl::notifyPressedButtonEvent);
		}

		// OBSOLETE
		assignWidget(mButtonDecor, "ButtonDecor");
		if (mButtonDecor != nullptr)
		{
			mButtonDecor->setVisible(false);
		}

		///@wskin_child{TabControl, Widget, TabItem} Шаблон для страницы, по которому будут создаваться клиентские зоны страниц.
		assignWidget(mItemTemplate, "TabItem");
		if (mItemTemplate != nullptr)
		{
			mItemTemplate->setVisible(false);
		}

#ifndef MYGUI_DONT_USE_OBSOLETE
		if (mItemTemplate == nullptr)
		{
			assignWidget(mItemTemplate, "Sheet");
			if (mItemTemplate != nullptr)
			{
				mItemTemplate->setVisible(false);
			}
		}
#endif // MYGUI_DONT_USE_OBSOLETE

		// OBSOLETE
		Widget* showPatch = nullptr;
		assignWidget(showPatch, "ShowPatch");
		if (showPatch != nullptr)
		{
			mWidgetsPatch.push_back(showPatch);
			showPatch->setVisible(false);
		}

		///@wskin_child{TabControl, Widget, HeaderPlace} Место для заголовоков.
		assignWidget(mHeaderPlace, "HeaderPlace");

		///@wskin_child{TabControl, Widget, Controls} Виджет на котором должны быть расположены кнопки влево и вправо для заголовоков.
		assignWidget(mControls, "Controls");

		///@wskin_child{TabControl, Widget, Empty} Виджет который будет показываться в месте где нет заголовков (справа от заголовков).
		assignWidget(mEmpty, "Empty");

		if (mEmpty == nullptr)
		{
			// создаем виджет, носитель скина пустоты бара
			// OBSOLETE
			mEmptyBarWidget = _getWidgetBar()->createWidget<Widget>(mEmptySkinName, IntCoord(), Align::Left | Align::Top);
		}

		updateBar();

		// FIXME добавленно, так как шетдаун вызывается и при смене скина
		mShutdown = false;
	}

	void TabControl::shutdownOverride()
	{
		mWidgetsPatch.clear();
		mWidgetBar = nullptr;
		mButtonLeft = nullptr;
		mButtonRight = nullptr;
		mButtonDecor = nullptr;
		mItemTemplate = nullptr;
		mEmptyBarWidget = nullptr;

		mHeaderPlace = nullptr;
		mControls = nullptr;
		mEmpty = nullptr;

		// FIXME перенесенно из деструктора, может косячить при смене скина
		mShutdown = true;

		Base::shutdownOverride();
	}

	void TabControl::onWidgetCreated(Widget* _widget)
	{
		Base::onWidgetCreated(_widget);

		TabItem* child = _widget->castType<TabItem>(false);
		if (child != nullptr)
		{
			child->setCoord(_getWidgetTemplate()->getAbsoluteLeft() - getAbsoluteLeft(), _getWidgetTemplate()->getAbsoluteTop() - getAbsoluteTop(), _getWidgetTemplate()->getWidth(), _getWidgetTemplate()->getHeight());
			child->setAlign(_getWidgetTemplate()->getAlign());

			_insertItem(ITEM_NONE, "", child, Any::Null);
		}
	}

	TabItem* TabControl::insertItemAt(size_t _index, const UString& _name, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "TabControl::insertItem");

		Widget* widget = Base::baseCreateWidget(WidgetStyle::Child, TabItem::getClassTypeName(), "Default", _getWidgetTemplate()->getCoord(), _getWidgetTemplate()->getAlign(), "", "", false);

		size_t lastIndex = mItemsInfo.size() - 1;
		setItemNameAt(lastIndex, _name);
		setItemDataAt(lastIndex, _data);

		swapItems(_index == ITEM_NONE ? lastIndex : _index, lastIndex);

		return widget->castType<TabItem>();
	}

	void TabControl::swapItems(size_t _index1, size_t _index2)
	{
		MYGUI_ASSERT_RANGE(_index1, mItemsInfo.size(), "TabControl::swapItems");
		MYGUI_ASSERT_RANGE(_index2, mItemsInfo.size(), "TabControl::swapItems");

		if (_index1 != _index2)
		{
			std::swap(mItemsInfo[_index1], mItemsInfo[_index2]);
			updateBar();
		}
	}

	void TabControl::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);

		updateBar();
	}

	void TabControl::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateBar();
	}

	void TabControl::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		updateBar();
	}

	void TabControl::notifyPressedButtonEvent(MyGUI::Widget* _sender)
	{
		if (_sender == mButtonLeft)
		{
			if (mStartIndex > 0)
			{
				mStartIndex --;
				updateBar();
			}
		}
		else if (_sender == mButtonRight)
		{
			if ((mStartIndex + 1) < mItemsInfo.size())
			{
				mStartIndex ++;
				// в updateBar() будет подкорректированно если что
				updateBar();
			}
		}
	}

	void TabControl::notifyPressedBarButtonEvent(MyGUI::Widget* _sender)
	{
		size_t select = *_sender->_getInternalData<size_t>() + mStartIndex;
		// щелкнули по той же кнопке
		if (select == mIndexSelect)
		{
			// стараемся показать выделенную кнопку
			beginToItemSelected();
			return;
		}
		size_t old = mIndexSelect;
		mIndexSelect = select;

		size_t count = 0;
		for (size_t pos = 0; pos < mItemButton.size(); pos++)
		{
			Button* button = mItemButton[count]->castType<Button>();
			if (button->getVisible())
			{
				// корректируем нажатость кнопки
				button->setStateSelected((pos + mStartIndex) == mIndexSelect);
			}
			count ++;
		}

		// стараемся показать выделенную кнопку
		beginToItemSelected();

		// поднимаем страницу для пикинга
		_forcePick(mItemsInfo[mIndexSelect].item);

		_showItem(mItemsInfo[mIndexSelect].item, true, mSmoothShow);
		_showItem(mItemsInfo[old].item, false, mSmoothShow);

		eventTabChangeSelect(this, mIndexSelect);
	}

	void TabControl::beginToItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "TabControl::beginToItemAt");

		// подстраховка
		if (_getWidgetBar()->getWidth() < 1)
			return;

		if (_index == mStartIndex)
			return;
		else if (_index < mStartIndex)
		{
			mStartIndex = _index;
			updateBar();
		}
		else
		{
			// длинна бара от старт индекса до нужной включительно
			int width = 0;
			for (size_t pos = mStartIndex; pos <= _index; pos++)
			{
				width += mItemsInfo[pos].width;
			}

			// уменьшем старт индекс пока не появиться нужная
			bool change = false;
			while ((mStartIndex < _index) && (width > _getWidgetBar()->getWidth()))
			{
				width -= mItemsInfo[mStartIndex].width;
				mStartIndex ++;
				change = true;
			}
			if (change)
				updateBar();
		}
	}

	void TabControl::setButtonDefaultWidth(int _width)
	{
		mButtonDefaultWidth = _width;
		if (mButtonDefaultWidth < 1)
			mButtonDefaultWidth = 1;
		setButtonAutoWidth(false);
	}

	void TabControl::setButtonAutoWidth(bool _auto)
	{
		mButtonAutoWidth = _auto;

		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			int width;
			if (mButtonAutoWidth)
				width = _getTextWidth(mItemsInfo[pos].name);
			else
				width = mButtonDefaultWidth;

			mWidthBar += width - mItemsInfo[pos].width;
			mItemsInfo[pos].width = width;
		}

		updateBar();
	}

	void TabControl::setButtonWidthAt(size_t _index, int _width)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "TabControl::setButtonWidthAt");

		if (_width <= 0)
		{
			if (mButtonAutoWidth)
				_width = _getTextWidth(mItemsInfo[_index].name);
			else
				_width = mButtonDefaultWidth;
		}

		mWidthBar += _width - mItemsInfo[_index].width;
		mItemsInfo[_index].width = _width;

		updateBar();
	}

	void TabControl::setItemNameAt(size_t _index, const UString& _name)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "TabControl::setItemNameAt");
		mItemsInfo[_index].name = _name;

		int width;
		if (mButtonAutoWidth)
			width = _getTextWidth(_name);
		else
			width = mButtonDefaultWidth;

		mWidthBar += width - mItemsInfo[_index].width;
		mItemsInfo[_index].width = width;

		updateBar();
	}

	void TabControl::setIndexSelected(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "TabControl::setIndexSelected");
		if (mIndexSelect == _index)
			return;
		size_t old = mIndexSelect;
		mIndexSelect = _index;
		updateBar();

		// поднимаем страницу для пикинга
		if (mSmoothShow)
			_forcePick(mItemsInfo[mIndexSelect].item);

		_showItem(mItemsInfo[mIndexSelect].item, true, mSmoothShow);
		_showItem(mItemsInfo[old].item, false, mSmoothShow);

		beginToItemSelected();
	}

	void TabControl::actionWidgetHide(Widget* _widget, ControllerItem* _controller)
	{
		_widget->setVisible(false);
		_widget->setEnabled(true);
	}

	void TabControl::_showItem(TabItem* _item, bool _show, bool _smooth)
	{
		if (!_smooth)
		{
			ControllerManager::getInstance().removeItem(_item);
			_item->setAlpha(ALPHA_MAX);

			_item->setVisible(_show);

			return;
		}

		if (_show)
		{
			ControllerFadeAlpha* controller = createControllerFadeAlpha(ALPHA_MAX, TAB_SPEED_FADE_COEF, true);
			ControllerManager::getInstance().addItem(_item, controller);
		}
		else
		{
			ControllerFadeAlpha* controller = createControllerFadeAlpha(ALPHA_MIN, TAB_SPEED_FADE_COEF, false);
			controller->eventPostAction += newDelegate(this, &TabControl::actionWidgetHide);
			ControllerManager::getInstance().addItem(_item, controller);
		}
	}

	Button* TabControl::createButton()
	{
		Widget* parent = this;
		if (mWidgetBar != nullptr)
			parent = mWidgetBar;
		else if (mHeaderPlace != nullptr)
			parent = mHeaderPlace;

		return parent->createWidget<Button>(mButtonSkinName, IntCoord(), Align::Left | Align::Top);
	}

	void TabControl::_createItemButton()
	{
		Button* button = createButton();
		button->eventMouseButtonClick += newDelegate(this, &TabControl::notifyPressedBarButtonEvent);
		button->_setInternalData(mItemButton.size()); // порядковый номер
		mItemButton.push_back(button);
	}

	int TabControl::_getTextWidth(const UString& _text)
	{
		if (mItemButton.empty())
			_createItemButton();

		UString save = mItemButton[0]->getCaption();
		mItemButton[0]->setCaption(_text);

		ISubWidgetText* text = mItemButton[0]->getSubWidgetText();
		const IntSize& size = text ? text->getTextSize() : IntSize();
		const IntCoord& coord = text ? text->getCoord() : IntCoord();

		mItemButton[0]->setCaption(save);

		return size.width + mItemButton[0]->getWidth() - coord.width;
	}

	void TabControl::_notifyDeleteItem(TabItem* _sheet)
	{
		// общий шутдаун виджета
		if (mShutdown)
			return;

		size_t index = getItemIndex(_sheet);

		mWidthBar -= mItemsInfo[index].width;
		mItemsInfo.erase(mItemsInfo.begin() + index);

		if (mItemsInfo.empty())
			mIndexSelect = ITEM_NONE;
		else
		{
			if (index < mIndexSelect)
				mIndexSelect --;
			else if (index == mIndexSelect)
			{
				if (mIndexSelect == mItemsInfo.size())
					mIndexSelect --;
				mItemsInfo[mIndexSelect].item->setVisible(true);
				mItemsInfo[mIndexSelect].item->setAlpha(ALPHA_MAX);
			}
		}

		updateBar();
	}

	void TabControl::_insertItem(size_t _index, const UString& _name, TabItem* _sheet, Any _data)
	{
		if (_index == ITEM_NONE)
			_index = mItemsInfo.size();

		// добавляем инфу о вкладке
		int width = (mButtonAutoWidth ? _getTextWidth(_name) : mButtonDefaultWidth);
		mWidthBar += width;

		mItemsInfo.insert(mItemsInfo.begin() + _index, TabItemInfo(width, _name, _sheet, _data));

		// первая вкладка
		if (1 == mItemsInfo.size())
			mIndexSelect = 0;
		else
		{
			_sheet->setVisible(false);
			if (_index <= mIndexSelect)
				mIndexSelect ++;
		}

		updateBar();
	}

	void TabControl::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "TabControl::setItemDataAt");
		mItemsInfo[_index].data = _data;
	}

	int TabControl::getButtonWidthAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "TabControl::getButtonWidthAt");
		return mItemsInfo[_index].width;
	}

	const UString& TabControl::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "TabControl::getItemNameAt");
		return mItemsInfo[_index].name;
	}

	TabItem* TabControl::getItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "TabControl::getItemAt");
		return mItemsInfo[_index].item;
	}

	void TabControl::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "TabControl::removeItemAt");
		this->_destroyChildWidget(mItemsInfo[_index].item);
	}

	void TabControl::removeAllItems()
	{
		while (!mItemsInfo.empty())
		{
			_destroyChildWidget(mItemsInfo.back().item);
		}
	}

	ControllerFadeAlpha* TabControl::createControllerFadeAlpha(float _alpha, float _coef, bool _enable)
	{
		ControllerItem* item = ControllerManager::getInstance().createItem(ControllerFadeAlpha::getClassTypeName());
		ControllerFadeAlpha* controller = item->castType<ControllerFadeAlpha>();

		controller->setAlpha(_alpha);
		controller->setCoef(_coef);
		controller->setEnabled(_enable);

		return controller;
	}

	size_t TabControl::getItemIndex(TabItem* _item)
	{
		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			if (mItemsInfo[pos].item == _item)
				return pos;
		}
		MYGUI_EXCEPT("item (" << _item << ") not found, source 'TabControl::getItemIndex'");
	}

	size_t TabControl::findItemIndex(TabItem* _item)
	{
		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			if (mItemsInfo[pos].item == _item)
				return pos;
		}
		return ITEM_NONE;
	}

	size_t TabControl::findItemIndexWith(const UString& _name)
	{
		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			if (mItemsInfo[pos].name == _name)
				return pos;
		}
		return ITEM_NONE;
	}

	TabItem* TabControl::findItemWith(const UString& _name)
	{
		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			if (mItemsInfo[pos].name == _name)
				return mItemsInfo[pos].item;
		}
		return nullptr;
	}

	TabItem* TabControl::getItemSelected()
	{
		return getIndexSelected() != ITEM_NONE ? getItemAt(getIndexSelected()) : nullptr;
	}

	Widget* TabControl::_getWidgetTemplate()
	{
		return mItemTemplate == nullptr ? this : mItemTemplate;
	}

	Widget* TabControl::_getWidgetBar()
	{
		return mWidgetBar == nullptr ? this : mWidgetBar;
	}

	void TabControl::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{TabControl, ButtonWidth, int} Ширина кнопок в заголовках в пикселях.
		if (_key == "ButtonWidth")
			setButtonDefaultWidth(utility::parseValue<int>(_value));

		/// @wproperty{TabControl, ButtonAutoWidth, bool} Режим автоматического вычисления ширины кнопок в заголовках.
		else if (_key == "ButtonAutoWidth")
			setButtonAutoWidth(utility::parseValue<bool>(_value));

		/// @wproperty{TabControl, SmoothShow, bool} Плавное переключение между закладками.
		else if (_key == "SmoothShow")
			setSmoothShow(utility::parseValue<bool>(_value));

		// не коментировать
		else if (_key == "SelectItem")
			setIndexSelected(utility::parseValue<size_t>(_value));

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	void TabControl::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void TabControl::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void TabControl::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

	size_t TabControl::getItemCount() const
	{
		return mItemsInfo.size();
	}

	TabItem* TabControl::insertItem(TabItem* _to, const UString& _name, Any _data)
	{
		return insertItemAt(getItemIndex(_to), _name, _data);
	}

	TabItem* TabControl::addItem(const UString& _name, Any _data)
	{
		return insertItemAt(ITEM_NONE, _name, _data);
	}

	void TabControl::removeItem(TabItem* _item)
	{
		removeItemAt(getItemIndex(_item));
	}

	size_t TabControl::getIndexSelected() const
	{
		return mIndexSelect;
	}

	void TabControl::setItemSelected(TabItem* _item)
	{
		setIndexSelected(getItemIndex(_item));
	}

	void TabControl::setItemData(TabItem* _item, Any _data)
	{
		setItemDataAt(getItemIndex(_item), _data);
	}

	void TabControl::clearItemDataAt(size_t _index)
	{
		setItemDataAt(_index, Any::Null);
	}

	void TabControl::clearItemData(TabItem* _item)
	{
		clearItemDataAt(getItemIndex(_item));
	}

	void TabControl::setItemName(TabItem* _item, const UString& _name)
	{
		setItemNameAt(getItemIndex(_item), _name);
	}

	const UString& TabControl::getItemName(TabItem* _item)
	{
		return getItemNameAt(getItemIndex(_item));
	}

	void TabControl::beginToItem(TabItem* _item)
	{
		beginToItemAt(getItemIndex(_item));
	}

	void TabControl::beginToItemFirst()
	{
		if (getItemCount())
			beginToItemAt(0);
	}

	void TabControl::beginToItemLast()
	{
		if (getItemCount())
			beginToItemAt(getItemCount() - 1);
	}

	void TabControl::beginToItemSelected()
	{
		if (getIndexSelected() != ITEM_NONE)
			beginToItemAt(getIndexSelected());
	}

	void TabControl::setButtonWidth(TabItem* _item, int _width)
	{
		setButtonWidthAt(getItemIndex(_item), _width);
	}

	int TabControl::getButtonWidth(TabItem* _item)
	{
		return getButtonWidthAt(getItemIndex(_item));
	}

	int TabControl::getButtonDefaultWidth() const
	{
		return mButtonDefaultWidth;
	}

	bool TabControl::getButtonAutoWidth() const
	{
		return mButtonAutoWidth;
	}

	void TabControl::setSmoothShow(bool _value)
	{
		mSmoothShow = _value;
	}

	bool TabControl::getSmoothShow() const
	{
		return mSmoothShow;
	}

	size_t TabControl::_getItemCount()
	{
		return getItemCount();
	}

	void TabControl::_addItem(const MyGUI::UString& _name)
	{
		addItem(_name);
	}

	void TabControl::_removeItemAt(size_t _index)
	{
		removeItemAt(_index);
	}

	Widget* TabControl::_getItemAt(size_t _index)
	{
		return getItemAt(_index);
	}

	void TabControl::_setItemNameAt(size_t _index, const UString& _name)
	{
		setItemNameAt(_index, _name);
	}

	const UString& TabControl::_getItemNameAt(size_t _index)
	{
		return getItemNameAt(_index);
	}

	void TabControl::updateBar()
	{
		if (mHeaderPlace != nullptr)
			updateBarNew();
		else
			updateBarOld();
	}

	void TabControl::updateBarOld()
	{
		// подстраховка
		if (_getWidgetBar()->getWidth() < 1)
			return;

		if ((_getWidgetBar()->getWidth() < mWidthBar) && (1 < mItemsInfo.size()))
		{
			if (!mButtonShow)
			{
				mButtonShow = true;

				if (nullptr != mButtonLeft)
					mButtonLeft->setVisible(true);
				if (nullptr != mButtonRight)
					mButtonRight->setVisible(true);
				if (nullptr != mButtonDecor)
					mButtonDecor->setVisible(true);
				for (VectorWidgetPtr::iterator iter = mWidgetsPatch.begin(); iter != mWidgetsPatch.end(); ++iter)
					(*iter)->setVisible(true);
				if (mWidgetBar != nullptr)
					mWidgetBar->setSize(mWidgetBar->getWidth() - mOffsetTab, mWidgetBar->getHeight());
			}
		}
		else
		{
			if (mButtonShow)
			{
				mButtonShow = false;
				if (nullptr != mButtonLeft)
					mButtonLeft->setVisible(false);
				if (nullptr != mButtonRight)
					mButtonRight->setVisible(false);
				if (nullptr != mButtonDecor)
					mButtonDecor->setVisible(false);
				for (VectorWidgetPtr::iterator iter = mWidgetsPatch.begin(); iter != mWidgetsPatch.end(); ++iter)
					(*iter)->setVisible(false);
				if (mWidgetBar != nullptr)
					mWidgetBar->setSize(mWidgetBar->getWidth() + mOffsetTab, mWidgetBar->getHeight());
			}
		}

		// проверяем правильность стартового индекса
		if (mStartIndex > 0)
		{
			// считаем длинну видимых кнопок
			int width = 0;
			for (size_t pos = mStartIndex; pos < mItemsInfo.size(); pos++)
				width += mItemsInfo[pos].width;

			// уменьшаем индекс до тех пор пока кнопка до индекста полностью не влезет в бар
			while ((mStartIndex > 0) && ((width + mItemsInfo[mStartIndex - 1].width) <= _getWidgetBar()->getWidth()))
			{
				mStartIndex--;
				width += mItemsInfo[mStartIndex].width;
			}
		}

		// проверяем и обновляем бар
		int width = 0;
		size_t count = 0;
		size_t pos = mStartIndex;
		for (; pos < mItemsInfo.size(); pos++)
		{
			// текущая кнопка не влазиет
			if (width > _getWidgetBar()->getWidth())
				break;

			// следующая не влазиет
			TabItemInfo& info = mItemsInfo[pos];
			if ((width + info.width) > _getWidgetBar()->getWidth())
			{
				break;
			}

			// проверяем физическое наличие кнопки
			if (count >= mItemButton.size())
				_createItemButton();

			// если кнопка не соответствует, то изменяем ее
			Button* button = mItemButton[count]->castType<Button>();
			button->setVisible(true);

			// корректируем нажатость кнопки
			button->setStateSelected(pos == mIndexSelect);

			if (button->getCaption() != info.name)
				button->setCaption(info.name);
			// положение кнопки
			IntCoord coord(width, 0, info.width, _getWidgetBar()->getHeight());
			if (coord != button->getCoord())
				button->setCoord(coord);

			width += info.width;
			count ++;
		}

		// скрываем кнопки что были созданны, но не видны
		while (count < mItemButton.size())
		{
			mItemButton[count]->setVisible(false);
			count ++;
		}

		bool right = true;
		if (pos == mItemsInfo.size())
			right = false;

		// в редакторе падает почему то, хотя этот скин создается всегда
		if (mEmptyBarWidget != nullptr)
		{
			// корректируем виджет для пустоты
			if (width < _getWidgetBar()->getWidth())
			{
				mEmptyBarWidget->setVisible(true);
				mEmptyBarWidget->setCoord(width, 0, _getWidgetBar()->getWidth() - width, _getWidgetBar()->getHeight());
			}
			else
			{
				mEmptyBarWidget->setVisible(false);
			}
		}

		// корректируем доступность стрелок
		if (mStartIndex == 0)
		{
			if (nullptr != mButtonLeft)
				mButtonLeft->setEnabled(false);
		}
		else
		{
			if (nullptr != mButtonLeft)
				mButtonLeft->setEnabled(true);
		}

		if (right)
		{
			if (nullptr != mButtonRight)
				mButtonRight->setEnabled(true);
		}
		else
		{
			if (nullptr != mButtonRight)
				mButtonRight->setEnabled(false);
		}
	}

	void TabControl::updateBarNew()
	{
		if (mHeaderPlace == nullptr)
			return;

		// подстраховка
		if (mHeaderPlace->getWidth() < 1)
			return;

		int widthControls = 0;
		if (mControls != nullptr)
			widthControls = mControls->getWidth();

		if ((mHeaderPlace->getWidth() < mWidthBar) && (1 < mItemsInfo.size()) && (mHeaderPlace->getWidth() >= widthControls))
		{
			if (!mButtonShow)
			{
				mButtonShow = true;

				if (nullptr != mControls)
					mControls->setVisible(true);
			}

			if (mControls != nullptr)
				mControls->setCoord(mHeaderPlace->getWidth() - mControls->getWidth(), 0, mControls->getWidth(), mHeaderPlace->getHeight());
		}
		else
		{
			if (mButtonShow)
			{
				mButtonShow = false;

				if (nullptr != mControls)
					mControls->setVisible(false);
			}

			widthControls = 0;
		}

		// проверяем правильность стартового индекса
		if (mStartIndex > 0)
		{
			// считаем длинну видимых кнопок
			int width = 0;
			for (size_t pos = mStartIndex; pos < mItemsInfo.size(); pos++)
				width += mItemsInfo[pos].width;

			// уменьшаем индекс до тех пор пока кнопка до индекста полностью не влезет в бар
			while ((mStartIndex > 0) && ((width + mItemsInfo[mStartIndex - 1].width) <= (mHeaderPlace->getWidth() - widthControls)))
			{
				mStartIndex--;
				width += mItemsInfo[mStartIndex].width;
			}
		}

		// проверяем и обновляем бар
		int width = 0;
		size_t count = 0;
		size_t pos = mStartIndex;
		for (; pos < mItemsInfo.size(); pos++)
		{
			// текущая кнопка не влазиет
			if (width > (mHeaderPlace->getWidth() - widthControls))
				break;

			// следующая не влазиет
			TabItemInfo& info = mItemsInfo[pos];
			if ((width + info.width) > (mHeaderPlace->getWidth() - widthControls))
			{
				break;
			}

			// проверяем физическое наличие кнопки
			if (count >= mItemButton.size())
				_createItemButton();

			// если кнопка не соответствует, то изменяем ее
			Button* button = mItemButton[count];
			button->setVisible(true);

			// корректируем нажатость кнопки
			button->setStateSelected(pos == mIndexSelect);

			if (button->getCaption() != info.name)
				button->setCaption(info.name);
			// положение кнопки
			IntCoord coord(width, 0, info.width, mHeaderPlace->getHeight());
			if (coord != button->getCoord())
				button->setCoord(coord);

			width += info.width;
			count ++;
		}

		// скрываем кнопки что были созданны, но не видны
		while (count < mItemButton.size())
		{
			mItemButton[count]->setVisible(false);
			count ++;
		}

		bool right = true;
		if (pos == mItemsInfo.size())
			right = false;

		if (mEmpty != nullptr)
		{
			// корректируем виджет для пустоты
			mEmpty->setCoord(width, 0, mHeaderPlace->getWidth() - width - widthControls, mHeaderPlace->getHeight());
		}

		// корректируем доступность стрелок
		if (mStartIndex == 0)
		{
			if (nullptr != mButtonLeft)
				mButtonLeft->setEnabled(false);
		}
		else
		{
			if (nullptr != mButtonLeft)
				mButtonLeft->setEnabled(true);
		}

		if (right)
		{
			if (nullptr != mButtonRight)
				mButtonRight->setEnabled(true);
		}
		else
		{
			if (nullptr != mButtonRight)
				mButtonRight->setEnabled(false);
		}
	}

} // namespace MyGUI
