/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	DDContainer::DDContainer() :
		mDropResult(false),
		mNeedDrop(false),
		mStartDrop(false),
		mOldDrop(nullptr),
		mCurrentSender(nullptr),
		mDropSenderIndex(ITEM_NONE),
		mDropItem(nullptr),
		mNeedDragDrop(false),
		mReseiverContainer(nullptr)
	{
	}

	void DDContainer::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		// смещение внутри виджета, куда кликнули мышкой
		mClickInWidget = InputManager::getInstance().getLastPressedPosition(MouseButton::Left) - getAbsolutePosition();

		mouseButtonPressed(_id);

		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void DDContainer::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		mouseButtonReleased(_id);

		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void DDContainer::onMouseDrag(int _left, int _top, MouseButton _id)
	{
		mouseDrag(_id);

		Base::onMouseDrag(_left, _top, _id);
	}

	void DDContainer::mouseButtonPressed(MouseButton _id)
	{
		if (MouseButton::Left == _id)
		{
			// сбрасываем инфу для дропа
			mDropResult = false;
			mOldDrop = nullptr;
			mDropInfo.reset();
			mReseiverContainer = nullptr;

			// сбрасываем, чтобы обновился дропный виджет
			mCurrentSender = nullptr;
			mStartDrop = false;

		}
		// если нажата другая клавиша и был дроп то сбрасываем
		else
		{
			endDrop(true);
		}
	}

	void DDContainer::mouseButtonReleased(MouseButton _id)
	{
		if (MouseButton::Left == _id)
		{
			endDrop(false);
		}
	}

	void DDContainer::mouseDrag(MouseButton _id)
	{
		if (MouseButton::Left != _id)
			return;

		// нужно ли обновить данные
		bool update = false;

		// первый раз дропаем елемент
		if (!mStartDrop && mDropSenderIndex != ITEM_NONE)
		{
			mStartDrop = true;
			mNeedDrop = false;
			update = true;
			// запрос на нужность дропа по индексу
			mDropInfo.set(this, mDropSenderIndex, nullptr, ITEM_NONE);
			mReseiverContainer = nullptr;

			eventStartDrag(this, mDropInfo, mNeedDrop);

			if (mNeedDrop)
			{
				eventChangeDDState(this, DDItemState::Start);
			}
			else
			{
				// сбрасываем фокус мыши (не обязательно)
				InputManager::getInstance().resetMouseCaptureWidget();
			}
		}

		// дроп не нужен
		if (!mNeedDrop)
		{
			return;
		}

		// делаем запрос, над кем наша мыша
		const IntPoint& point = InputManager::getInstance().getMousePosition();
		Widget* item = LayerManager::getInstance().getWidgetFromPoint(point.left, point.top);

		updateDropItems();

		// если равно, значит уже спрашивали
		if (mOldDrop == item) return;
		mOldDrop = item;

		// сбрасываем старую подсветку
		if (mReseiverContainer) mReseiverContainer->_setContainerItemInfo(mDropInfo.receiver_index, false, false);

		mDropResult = false;
		mReseiverContainer = nullptr;
		Widget* receiver = nullptr;
		size_t receiver_index = ITEM_NONE;
		// есть виджет под нами
		if (item)
		{
			// делаем запрос на индекс по произвольному виджету
			receiver = item->_getContainer();
			// работаем только с контейнерами
			if (receiver && receiver->isType<DDContainer>())
			{
				receiver_index = receiver->_getItemIndex(item);

				// подписываемся на информацию о валидности дропа
				mReseiverContainer = static_cast<DDContainer*>(receiver);
				mReseiverContainer->_eventInvalideContainer.clear();
				mReseiverContainer->_eventInvalideContainer += newDelegate(this, &DDContainer::notifyInvalideDrop);

				// делаем запрос на возможность дропа
				mDropInfo.set(this, mDropSenderIndex, mReseiverContainer, receiver_index);

				eventRequestDrop(this, mDropInfo, mDropResult);

				// устанавливаем новую подсветку
				mReseiverContainer->_setContainerItemInfo(mDropInfo.receiver_index, true, mDropResult);
			}
			else
			{
				mDropInfo.set(this, mDropSenderIndex, nullptr, ITEM_NONE);
			}
		}
		// нет виджета под нами
		else
		{
			mDropInfo.set(this, mDropSenderIndex, nullptr, ITEM_NONE);
		}

		DDItemState state;

		DDWidgetState data(mDropSenderIndex);
		data.update = update;

		if (receiver == nullptr)
		{
			data.accept = false;
			data.refuse = false;
			state = DDItemState::Miss;
		}
		else if (mDropResult)
		{
			data.accept = true;
			data.refuse = false;
			state = DDItemState::Accept;
		}
		else
		{
			data.accept = false;
			data.refuse = true;
			state = DDItemState::Refuse;
		}

		updateDropItemsState(data);

		eventChangeDDState(this, state);
	}

	void DDContainer::endDrop(bool _reset)
	{
		if (mStartDrop)
		{
			removeDropItems();

			// сбрасываем старую подсветку
			if (mReseiverContainer) mReseiverContainer->_setContainerItemInfo(mDropInfo.receiver_index, false, false);

			if (_reset) mDropResult = false;
			eventDropResult(this, mDropInfo, mDropResult);
			eventChangeDDState(this, DDItemState::End);

			// сбрасываем инфу для дропа
			mStartDrop = false;
			mDropResult = false;
			mNeedDrop = false;
			mOldDrop = nullptr;
			mDropInfo.reset();
			mReseiverContainer = nullptr;
			mDropSenderIndex = ITEM_NONE;
		}
	}

	void DDContainer::removeDropItems()
	{
		mDropItem = nullptr;
	}

	void DDContainer::updateDropItems()
	{

		if (mDropItem == nullptr)
		{
			requestDragWidgetInfo(this, mDropItem, mDropDimension);
		}

		const IntPoint& point = InputManager::getInstance().getMousePositionByLayer();

		if (mDropItem)
		{
			mDropItem->setCoord(point.left - mClickInWidget.left + mDropDimension.left, point.top - mClickInWidget.top + mDropDimension.top, mDropDimension.width, mDropDimension.height);
			mDropItem->setVisible(true);
		}
	}

	void DDContainer::updateDropItemsState(const DDWidgetState& _state)
	{
		eventUpdateDropState(this, mDropItem, _state);
	}

	void DDContainer::notifyInvalideDrop(DDContainer* _sender)
	{
		mouseDrag(MouseButton::Left);
	}

	void DDContainer::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{DDContainer, NeedDragDrop, bool} Поддержка Drag and Drop.
		if (_key == "NeedDragDrop")
			setNeedDragDrop(utility::parseValue<bool>(_value));

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	void DDContainer::setNeedDragDrop(bool _value)
	{
		mNeedDragDrop = _value;
	}

	bool DDContainer::getNeedDragDrop() const
	{
		return mNeedDragDrop;
	}

	void DDContainer::_setContainerItemInfo(size_t _index, bool _set, bool _accept)
	{
	}

	void DDContainer::resetDrag()
	{
		_resetContainer(false);
	}

} // namespace MyGUI
