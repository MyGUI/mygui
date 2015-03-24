/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ListBox.h"
#include "MyGUI_Button.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	const float COMBO_ALPHA_MAX  = ALPHA_MAX;
	const float COMBO_ALPHA_MIN  = ALPHA_MIN;
	const float COMBO_ALPHA_COEF = 4.0f;

	ComboBox::ComboBox() :
		mButton(nullptr),
		mList(nullptr),
		mListShow(false),
		mMaxListLength(-1),
		mItemIndex(ITEM_NONE),
		mModeDrop(false),
		mDropMouse(false),
		mShowSmooth(false),
		mFlowDirection(FlowDirection::TopToBottom)
	{
	}

	void ComboBox::initialiseOverride()
	{
		Base::initialiseOverride();

		///@wskin_child{ComboBox, Button, Button} Кнопка для выпадающего списка.
		assignWidget(mButton, "Button");
		if (mButton != nullptr)
		{
			mButton->eventMouseButtonPressed += newDelegate(this, &ComboBox::notifyButtonPressed);
		}

		///@wskin_child{ComboBox, ListBox, List} Выпадающий список.
		assignWidget(mList, "List");

		if (mList == nullptr)
		{
			std::string list_skin = getUserString("ListSkin");
			std::string list_layer = getUserString("ListLayer");

			mList = static_cast<ListBox*>(_createSkinWidget(WidgetStyle::Popup, ListBox::getClassTypeName(), list_skin, IntCoord(), Align::Default, list_layer));
		}

		if (mList != nullptr)
		{
			mList->setActivateOnClick(true);

			mList->setVisible(false);
			mList->eventKeyLostFocus += newDelegate(this, &ComboBox::notifyListLostFocus);
			mList->eventListSelectAccept += newDelegate(this, &ComboBox::notifyListSelectAccept);
			mList->eventListMouseItemActivate += newDelegate(this, &ComboBox::notifyListMouseItemActivate);
			mList->eventListChangePosition += newDelegate(this, &ComboBox::notifyListChangePosition);

			mList->setNeedToolTip(true);
			mList->eventToolTip += newDelegate(this, &ComboBox::notifyToolTip);
		}

		// подписываем дочерние классы на скролл
		if (mClient != nullptr)
		{
			mClient->eventMouseWheel += newDelegate(this, &ComboBox::notifyMouseWheel);
			mClient->eventMouseButtonPressed += newDelegate(this, &ComboBox::notifyMousePressed);

			mClient->setNeedToolTip(true);
			mClient->eventToolTip += newDelegate(this, &ComboBox::notifyToolTip);
		}

		// подписываемся на изменения текста
		eventEditTextChange += newDelegate(this, &ComboBox::notifyEditTextChange);
	}

	void ComboBox::shutdownOverride()
	{
		mList = nullptr;
		mButton = nullptr;
		mClient = nullptr;

		Base::shutdownOverride();
	}

	void ComboBox::notifyButtonPressed(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (MouseButton::Left != _id)
			return;

		mDropMouse = true;

		if (mListShow)
			hideList();
		else
			showList();
	}

	void ComboBox::notifyListLostFocus(Widget* _sender, Widget* _new)
	{
		if (mDropMouse)
		{
			mDropMouse = false;
			Widget* focus = InputManager::getInstance().getMouseFocusWidget();

			// кнопка сама уберет список
			if (focus == mButton)
				return;

			// в режиме дропа все окна учавствуют
			if (mModeDrop && focus == mClient)
				return;
		}

		hideList();
	}

	void ComboBox::notifyListSelectAccept(ListBox* _widget, size_t _position)
	{
		mItemIndex = _position;
		Base::setCaption(mItemIndex != ITEM_NONE ? mList->getItemNameAt(mItemIndex) : "");

		mDropMouse = false;
		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop)
		{
			_resetContainer(false);

			eventComboAccept.m_eventObsolete(this);
			eventComboAccept.m_event(this, mItemIndex);
		}
	}

	void ComboBox::notifyListChangePosition(ListBox* _widget, size_t _position)
	{
		mItemIndex = _position;

		_resetContainer(false);

		eventComboChangePosition(this, _position);
	}

	void ComboBox::onKeyButtonPressed(KeyCode _key, Char _char)
	{
		Base::onKeyButtonPressed(_key, _char);

		// при нажатии вниз, показываем лист
		if (_key == KeyCode::ArrowDown)
		{
			// выкидываем список только если мыша свободна
			if (!InputManager::getInstance().isCaptureMouse())
			{
				showList();
			}
		}
		// нажат ввод в окне редиктирования
		else if ((_key == KeyCode::Return) || (_key == KeyCode::NumpadEnter))
		{
			_resetContainer(false);

			eventComboAccept.m_eventObsolete(this);
			eventComboAccept.m_event(this, mItemIndex);
		}
	}

	void ComboBox::notifyListMouseItemActivate(ListBox* _widget, size_t _position)
	{
		mItemIndex = _position;
		Base::setCaption(mItemIndex != ITEM_NONE ? mList->getItemNameAt(mItemIndex) : "");

		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop)
		{
			_resetContainer(false);

			eventComboAccept.m_eventObsolete(this);
			eventComboAccept.m_event(this, mItemIndex);
		}
	}

	void ComboBox::notifyMouseWheel(Widget* _sender, int _rel)
	{
		if (mList->getItemCount() == 0)
			return;
		if (InputManager::getInstance().getKeyFocusWidget() != this)
			return;
		if (InputManager::getInstance().isCaptureMouse())
			return;

		if (_rel > 0)
		{
			if (mItemIndex != 0)
			{
				if (mItemIndex == ITEM_NONE)
					mItemIndex = 0;
				else
					mItemIndex --;
				Base::setCaption(mList->getItemNameAt(mItemIndex));
				mList->setIndexSelected(mItemIndex);
				mList->beginToItemAt(mItemIndex);

				_resetContainer(false);

				eventComboChangePosition(this, mItemIndex);
			}
		}
		else if (_rel < 0)
		{
			if ((mItemIndex + 1) < mList->getItemCount())
			{
				if (mItemIndex == ITEM_NONE)
					mItemIndex = 0;
				else
					mItemIndex ++;
				Base::setCaption(mList->getItemNameAt(mItemIndex));
				mList->setIndexSelected(mItemIndex);
				mList->beginToItemAt(mItemIndex);

				_resetContainer(false);

				eventComboChangePosition(this, mItemIndex);
			}
		}
	}

	void ComboBox::notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		// обязательно отдаем отцу, а то мы у него в наглую отняли
		Base::notifyMousePressed(_sender, _left, _top, _id);

		mDropMouse = true;

		// показываем список
		if (mModeDrop)
			notifyButtonPressed(nullptr, _left, _top, _id);
	}

	void ComboBox::notifyEditTextChange(EditBox* _sender)
	{
		// сбрасываем выделенный элемент
		if (ITEM_NONE != mItemIndex)
		{
			mItemIndex = ITEM_NONE;
			mList->setIndexSelected(mItemIndex);
			mList->beginToItemFirst();

			_resetContainer(false);

			eventComboChangePosition(this, mItemIndex);
		}
	}

	void ComboBox::showList()
	{
		// пустой список не показываем
		if (mList->getItemCount() == 0)
			return;

		mListShow = true;

		IntCoord coord = calculateListPosition();
		mList->setCoord(coord);

		if (mShowSmooth)
		{
			ControllerFadeAlpha* controller = createControllerFadeAlpha(COMBO_ALPHA_MAX, COMBO_ALPHA_COEF, true);
			ControllerManager::getInstance().addItem(mList, controller);
		}
		else
		{
			mList->setVisible(true);
		}

		InputManager::getInstance().setKeyFocusWidget(mList);
	}

	void ComboBox::actionWidgetHide(Widget* _widget, ControllerItem* _controller)
	{
		_widget->setVisible(false);
		_widget->setEnabled(true);
	}

	void ComboBox::hideList()
	{
		mListShow = false;

		if (mShowSmooth)
		{
			ControllerFadeAlpha* controller = createControllerFadeAlpha(COMBO_ALPHA_MIN, COMBO_ALPHA_COEF, false);
			controller->eventPostAction += newDelegate(this, &ComboBox::actionWidgetHide);
			ControllerManager::getInstance().addItem(mList, controller);
		}
		else
		{
			mList->setVisible(false);
		}
	}

	void ComboBox::setIndexSelected(size_t _index)
	{
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mList->getItemCount(), "ComboBox::setIndexSelected");
		mItemIndex = _index;
		mList->setIndexSelected(_index);
		if (_index == ITEM_NONE)
		{
			Base::setCaption("");
			return;
		}
		Base::setCaption(mList->getItemNameAt(_index));
		Base::updateView(); // hook for update
	}

	void ComboBox::setItemNameAt(size_t _index, const UString& _name)
	{
		mList->setItemNameAt(_index, _name);
		mItemIndex = ITEM_NONE;//FIXME
		mList->setIndexSelected(mItemIndex);//FIXME
	}

	void ComboBox::setItemDataAt(size_t _index, Any _data)
	{
		mList->setItemDataAt(_index, _data);
		mItemIndex = ITEM_NONE;//FIXME
		mList->setIndexSelected(mItemIndex);//FIXME
	}

	void ComboBox::insertItemAt(size_t _index, const UString& _item, Any _data)
	{
		mList->insertItemAt(_index, _item, _data);
		mItemIndex = ITEM_NONE;//FIXME
		mList->setIndexSelected(mItemIndex);//FIXME
	}

	void ComboBox::removeItemAt(size_t _index)
	{
		mList->removeItemAt(_index);
		mItemIndex = ITEM_NONE;//FIXME
		mList->clearIndexSelected();//FIXME
	}

	void ComboBox::removeAllItems()
	{
		mItemIndex = ITEM_NONE;//FIXME
		mList->removeAllItems();//FIXME заново созданные строки криво стоят
	}

	void ComboBox::setComboModeDrop(bool _drop)
	{
		mModeDrop = _drop;
		setEditStatic(mModeDrop);
	}

	ControllerFadeAlpha* ComboBox::createControllerFadeAlpha(float _alpha, float _coef, bool _enable)
	{
		ControllerItem* item = ControllerManager::getInstance().createItem(ControllerFadeAlpha::getClassTypeName());
		ControllerFadeAlpha* controller = item->castType<ControllerFadeAlpha>();

		controller->setAlpha(_alpha);
		controller->setCoef(_coef);
		controller->setEnabled(_enable);

		return controller;
	}

	size_t ComboBox::findItemIndexWith(const UString& _name)
	{
		return mList->findItemIndexWith(_name);
	}

	void ComboBox::setFlowDirection(FlowDirection _value)
	{
		mFlowDirection = _value;
	}

	IntCoord ComboBox::calculateListPosition()
	{
		int length = 0;
		if (mFlowDirection.isVertical())
			length = mList->getOptimalHeight();
		else
			length = mMaxListLength;

		if (mMaxListLength > 0 && length > mMaxListLength)
			length = mMaxListLength;

		// берем глобальные координаты выджета
		IntCoord coord = getAbsoluteCoord();
		// размер леера
		IntSize sizeView = mList->getParentSize();

		if (mFlowDirection == FlowDirection::TopToBottom)
		{
			if ((coord.bottom() + length) <= sizeView.height)
				coord.top += coord.height;
			else
				coord.top -= length;
			coord.height = length;
		}
		else if (mFlowDirection == FlowDirection::BottomToTop)
		{
			if ((coord.top - length) >= 0)
				coord.top -= length;
			else
				coord.top += coord.height;
			coord.height = length;
		}
		else if (mFlowDirection == FlowDirection::LeftToRight)
		{
			if ((coord.right() + length) <= sizeView.width)
				coord.left += coord.width;
			else
				coord.left -= length;
			coord.width = length;
		}
		else if (mFlowDirection == FlowDirection::RightToLeft)
		{
			if ((coord.left - length) >= 0)
				coord.left -= length;
			else
				coord.left += coord.width;
			coord.width = length;
		}

		return coord;
	}

	void ComboBox::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{ComboBox, ModeDrop, bool} Режим выпадающего списка, в этом режиме значение в поля поменять нельзя.
		if (_key == "ModeDrop")
			setComboModeDrop(utility::parseValue<bool>(_value));

		/// @wproperty{ComboBox, FlowDirection, FlowDirection} Направление выпадения списка.
		else if (_key == "FlowDirection")
			setFlowDirection(utility::parseValue<FlowDirection>(_value));

		/// @wproperty{ComboBox, MaxListLength, int} Максимальная высота или ширина (зависит от направления) списка в пикселях.
		else if (_key == "MaxListLength")
			setMaxListLength(utility::parseValue<int>(_value));

		/// @wproperty{ComboBox, SmoothShow, bool} Плавное раскрытие списка.
		else if (_key == "SmoothShow")
			setSmoothShow(utility::parseValue<bool>(_value));

		// не коментировать
		else if (_key == "AddItem")
			addItem(_value);

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	size_t ComboBox::getItemCount() const
	{
		return mList->getItemCount();
	}

	void ComboBox::addItem(const UString& _name, Any _data)
	{
		return insertItemAt(ITEM_NONE, _name, _data);
	}

	size_t ComboBox::getIndexSelected() const
	{
		return mItemIndex;
	}

	void ComboBox::clearIndexSelected()
	{
		setIndexSelected(ITEM_NONE);
	}

	void ComboBox::clearItemDataAt(size_t _index)
	{
		setItemDataAt(_index, Any::Null);
	}

	const UString& ComboBox::getItemNameAt(size_t _index)
	{
		return mList->getItemNameAt(_index);
	}

	void ComboBox::beginToItemAt(size_t _index)
	{
		mList->beginToItemAt(_index);
	}

	void ComboBox::beginToItemFirst()
	{
		if (getItemCount())
			beginToItemAt(0);
	}

	void ComboBox::beginToItemLast()
	{
		if (getItemCount())
			beginToItemAt(getItemCount() - 1);
	}

	void ComboBox::beginToItemSelected()
	{
		if (getIndexSelected() != ITEM_NONE)
			beginToItemAt(getIndexSelected());
	}

	bool ComboBox::getComboModeDrop() const
	{
		return mModeDrop;
	}

	void ComboBox::setSmoothShow(bool _value)
	{
		mShowSmooth = _value;
	}

	bool ComboBox::getSmoothShow() const
	{
		return mShowSmooth;
	}

	void ComboBox::setMaxListLength(int _value)
	{
		mMaxListLength = _value;
	}

	int ComboBox::getMaxListLength() const
	{
		return mMaxListLength;
	}

	FlowDirection ComboBox::getFlowDirection() const
	{
		return mFlowDirection;
	}

	void ComboBox::notifyToolTip(Widget* _sender, const ToolTipInfo& _info)
	{
		if (getNeedToolTip())
			eventToolTip(this, _info);
	}

	size_t ComboBox::_getItemCount()
	{
		return getItemCount();
	}

	void ComboBox::_addItem(const MyGUI::UString& _name)
	{
		addItem(_name);
	}

	void ComboBox::_removeItemAt(size_t _index)
	{
		removeItemAt(_index);
	}

	void ComboBox::_setItemNameAt(size_t _index, const UString& _name)
	{
		setItemNameAt(_index, _name);
	}

	const UString& ComboBox::_getItemNameAt(size_t _index)
	{
		return getItemNameAt(_index);
	}

	void ComboBox::_resetContainer(bool _update)
	{
		Base::_resetContainer(_update);
		if (mList != nullptr)
			mList->_resetContainer(_update);
	}

} // namespace MyGUI
