/*!
	@file
	@author		Albert Semenov
	@date		12/2007
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
#include "MyGUI_ComboBox.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_List.h"
#include "MyGUI_Button.h"
#include "MyGUI_ResourceSkin.h"

namespace MyGUI
{

	const float COMBO_ALPHA_MAX  = ALPHA_MAX;
	const float COMBO_ALPHA_MIN  = ALPHA_MIN;
	const float COMBO_ALPHA_COEF = 4.0f;

	ComboBox::ComboBox() :
		mButton(nullptr),
		mList(nullptr),
		mListShow(false),
		mMaxHeight(0),
		mItemIndex(ITEM_NONE),
		mModeDrop(false),
		mDropMouse(false),
		mShowSmooth(false),
		mManualList(true)
	{
	}

	void ComboBox::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
	{
		Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);

		initialiseWidgetSkin(_info);
	}

	ComboBox::~ComboBox()
	{
		shutdownWidgetSkin();
	}

	void ComboBox::baseChangeWidgetSkin(ResourceSkin* _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void ComboBox::initialiseWidgetSkin(ResourceSkin* _info)
	{
		// парсим свойства
		const MapString& properties = _info->getProperties();
		if (!properties.empty())
		{
			MapString::const_iterator iter = properties.find("HeightList");
			if (iter != properties.end()) mMaxHeight = utility::parseValue<int>(iter->second);

			iter = properties.find("ListSmoothShow");
			if (iter != properties.end()) setSmoothShow(utility::parseBool(iter->second));
		}

		// парсим кнопку
		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter)
		{
			if (*(*iter)->_getInternalData<std::string>() == "Button")
			{
				MYGUI_DEBUG_ASSERT( ! mButton, "widget already assigned");
				mButton = (*iter)->castType<Button>();
			}
			else if (*(*iter)->_getInternalData<std::string>() == "List")
			{
				MYGUI_DEBUG_ASSERT( ! mList, "widget already assigned");
				mList = (*iter)->castType<List>();
				mList->setVisible(false);
				mList->eventKeyLostFocus = newDelegate(this, &ComboBox::notifyListLostFocus);
				mList->eventListSelectAccept = newDelegate(this, &ComboBox::notifyListSelectAccept);
				mList->eventListMouseItemActivate = newDelegate(this, &ComboBox::notifyListMouseItemActivate);
				mList->eventListChangePosition = newDelegate(this, &ComboBox::notifyListChangePosition);
			}
		}

		mManualList = (mList == nullptr);
		if (mList == nullptr)
		{
			std::string list_skin;
			MapString::const_iterator iter = properties.find("ListSkin");
			if (iter != properties.end()) list_skin = iter->second;
			std::string list_layer;
			iter = properties.find("ListLayer");
			if (iter != properties.end()) list_layer = iter->second;
			mList = createWidget<MyGUI::List>(WidgetStyle::Popup, list_skin, IntCoord(), Align::Default, list_layer);
			mWidgetChild.pop_back();

			mList->setVisible(false);
			mList->eventKeyLostFocus = newDelegate(this, &ComboBox::notifyListLostFocus);
			mList->eventListSelectAccept = newDelegate(this, &ComboBox::notifyListSelectAccept);
			mList->eventListMouseItemActivate = newDelegate(this, &ComboBox::notifyListMouseItemActivate);
			mList->eventListChangePosition = newDelegate(this, &ComboBox::notifyListChangePosition);
		}

		// подписываемся на изменения текста
		eventEditTextChange = newDelegate(this, &ComboBox::notifyEditTextChange);
	}

	void ComboBox::shutdownWidgetSkin()
	{
		if (mManualList)
		{
			mWidgetChild.push_back(mList);
			WidgetManager::getInstance().destroyWidget(mList);
		}
		mList = nullptr;
		mButton = nullptr;
	}


	void ComboBox::changeDropState()
	{
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
			if ((mModeDrop) && (focus == mWidgetClient))
				return;
		}

		hideList();
	}

	void ComboBox::notifyListSelectAccept(List* _widget, size_t _position)
	{
		mItemIndex = _position;
		Base::setCaption(mItemIndex != ITEM_NONE ? mList->getItemNameAt(mItemIndex) : "");

		mDropMouse = false;
		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop)
		{
			eventComboAccept.m_eventObsolete(this);
			eventComboAccept.m_event(this, mItemIndex);
		}
	}

	void ComboBox::notifyListChangePosition(List* _widget, size_t _position)
	{
		mItemIndex = _position;
		eventComboChangePosition(this, _position);
	}

	void ComboBox::onEventKeyButtonDown(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args)
	{
		bool handled = false;

		if (_info->getSource() == this)
		{
			_info->setHandled(true);

			// при нажатии вниз, показываем лист
			if (_args->getKeyCode() == KeyCode::ArrowDown)
			{
				// выкидываем список только если мыша свободна
				if (!InputManager::getInstance().isCaptureMouse())
				{
					handled = true;

					showList();
				}
			}
			// нажат ввод в окне редиктирования
			else if ((_args->getKeyCode() == KeyCode::Return) || (_args->getKeyCode() == KeyCode::NumpadEnter))
			{
				handled = true;

				eventComboAccept.m_eventObsolete(this);
				eventComboAccept.m_event(this, mItemIndex);
			}
		}

		if (!handled)
			Base::onEventKeyButtonDown(_sender, _info, _args);
	}

	void ComboBox::notifyListMouseItemActivate(List* _widget, size_t _position)
	{
		mItemIndex = _position;
		Base::setCaption(mItemIndex != ITEM_NONE ? mList->getItemNameAt(mItemIndex) : "");

		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop)
		{
			eventComboAccept.m_eventObsolete(this);
			eventComboAccept.m_event(this, mItemIndex);
		}
	}

	void ComboBox::onEventMouseButtonClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args)
	{
		bool handled = false;

		// Button потом заюзать его метод клик
		if (_info->getSource() == mClient || _info->getSource() == mButton)
		{
			if (_args->getButton() == MouseButton::Left)
			{
				_info->setHandled(true);
				handled = true;

				if (mModeDrop || _info->getSource() == mButton)
				{
					mDropMouse = true;

					changeDropState();
				}
			}
		}

		if (!handled)
			Base::onEventMouseButtonClick(_sender, _info, _args);
	}

	void ComboBox::scrollCaption(int _delta)
	{
		if (_delta > 0)
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

				eventComboChangePosition(this, mItemIndex);
			}
		}
		else if (_delta < 0)
		{
			if ((mItemIndex+1) < mList->getItemCount())
			{
				if (mItemIndex == ITEM_NONE)
					mItemIndex = 0;
				else
					mItemIndex ++;

				Base::setCaption(mList->getItemNameAt(mItemIndex));
				mList->setIndexSelected(mItemIndex);
				mList->beginToItemAt(mItemIndex);

				eventComboChangePosition(this, mItemIndex);
			}
		}
	}

	void ComboBox::notifyEditTextChange(Edit* _sender)
	{
		// сбрасываем выделенный элемент
		if (ITEM_NONE != mItemIndex)
		{
			mItemIndex = ITEM_NONE;
			mList->setIndexSelected(mItemIndex);
			mList->beginToItemFirst();
			eventComboChangePosition(this, mItemIndex);
		}
	}

	void ComboBox::showList()
	{
		// пустой список не показываем
		if (mList->getItemCount() == 0)
			return;

		mListShow = true;

		int height = mList->getOptimalHeight();
		if (height > mMaxHeight) height = mMaxHeight;

		// берем глобальные координаты выджета
		IntCoord coord = getAbsoluteCoord();

		//показываем список вверх
		if ((coord.top + coord.height + height) > mList->getParentSize().height)
		{
			coord.height = height;
			coord.top -= coord.height;
		}
		// показываем список вниз
		else
		{
			coord.top += coord.height;
			coord.height = height;
		}
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

	void ComboBox::actionWidgetHide(Widget* _widget)
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
			controller->eventPostAction = newDelegate(this, &ComboBox::actionWidgetHide);
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

	void ComboBox::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "ComboBox_ModeDrop") setComboModeDrop(utility::parseValue<bool>(_value));
		else if (_key == "ComboBox_AddItem") addItem(_value);
		else
		{
			Base::setProperty(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

	void ComboBox::onEventMouseWheel(Widget* _sender, EventInfo* _info, MouseWheelEventArgs* _args)
	{
		bool handled = false;

		if (_info->getSource() == mClient)
		{
			_info->setHandled(true);

			bool need =
				mList->getItemCount() != 0
				&& InputManager::getInstance().getKeyFocusWidget() == this
				&& !InputManager::getInstance().isCaptureMouse();

			if (need)
			{
				handled = true;

				scrollCaption(_args->getDelta());
			}
		}

		if (!handled)
			Base::onEventMouseWheel(_sender, _info, _args);
	}

} // namespace MyGUI
