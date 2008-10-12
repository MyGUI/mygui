/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#include "MyGUI_DDContainer.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( DDContainer, Widget );

	DDContainer::DDContainer(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mDropResult(false),
		mStartDrop(false),
		mNeedDrop(false),
		mOldDrop(null),
		mCurrentSender(null),
		mDropSenderIndex(ITEM_NONE)
	{
	}

	void DDContainer::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		// смещение внутри виджета, куда кликнули мышкой
		mClickInWidget = InputManager::getInstance().getLastLeftPressed() - getAbsolutePosition();

		mouseButtonPressed(_id);
		Widget::onMouseButtonPressed(_left, _top, _id);
	}

	void DDContainer::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		mouseButtonReleased(_id);
		Widget::onMouseButtonReleased(_left, _top, _id);
	}

	void DDContainer::onMouseDrag(int _left, int _top)
	{
		mouseDrag();
		Widget::onMouseDrag(_left, _top);
	}

	void DDContainer::mouseButtonPressed(MouseButton _id)
	{
		if (MB_Left == _id) {
			// сбрасываем инфу для дропа
			mDropResult = false;
			mOldDrop = null;
			mDropInfo.reset();

			// сбрасываем, чтобы обновился дропный виджет
			mCurrentSender = null;
			mStartDrop = false;

		}
		// если нажата другая клавиша и был дроп то сбрасываем
		else {
			endDrop(true);
		}
	}

	void DDContainer::mouseButtonReleased(MouseButton _id)
	{
		if (MB_Left == _id) {
			endDrop(false);
		}
	}

	void DDContainer::mouseDrag()
	{
		// нужно ли обновить данные
		bool update = false;

		// первый раз дропаем елемент
		if (false == mStartDrop) {
			mStartDrop = true;
			mNeedDrop = false;
			update = true;
			// запрос на нужность дропа по индексу
			mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
			eventStartDrop(this, mDropInfo, mNeedDrop);

			if (mNeedDrop) {
				eventDropState(this, DROP_START);
				enableToolTip(false);
			}
			else {
				// сбрасываем фокус мыши (не обязательно)
				InputManager::getInstance().resetMouseCaptureWidget();
			}
		}

		// дроп не нужен
		if (false == mNeedDrop) {
			return;
		}

		// делаем запрос, над кем наша мыша
		const IntPoint & point = InputManager::getInstance().getMousePosition();
		WidgetPtr item = InputManager::getInstance().getWidgetFromPoint(point.left, point.top);

		updateDropItems();

		// если равно, значит уже спрашивали
		if (mOldDrop == item) return;
		mOldDrop = item;

		// сбрасываем старую подсветку
		if (mDropInfo.reseiver) mDropInfo.reseiver->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

		mDropResult = false;
		WidgetPtr reseiver = null;
		size_t reseiver_index = ITEM_NONE;
		// есть виджет под нами
		if (item) {
			// делаем запрос на индекс по произвольному виджету
			item->getContainer(reseiver, reseiver_index);
			if (reseiver) {
				// подписываемся на информацию о валидности дропа
				reseiver->eventInvalideContainer = newDelegate(this, &DDContainer::notifyInvalideDrop);

				// делаем запрос на возможность дропа
				mDropInfo.set(this, mDropSenderIndex, reseiver, reseiver_index);
				eventRequestDrop(this, mDropInfo, mDropResult);

				// устанавливаем новую подсветку
				mDropInfo.reseiver->setContainerItemInfo(mDropInfo.reseiver_index, true, mDropResult);
			}
			else {
				mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
			}
		}
		// нет виджета под нами
		else {
			mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
		}

		DropState state;

		DropWidgetState data(mDropSenderIndex);
		if (reseiver == null) {
			data.accept = false;
			data.refuse = false;
			state = DROP_MISS;
		}
		else if (mDropResult) {
			data.accept = true;
			data.refuse = false;
			state = DROP_ACCEPT;
		}
		else {
			data.accept = false;
			data.refuse = true;
			state = DROP_REFUSE;
		}

		updateDropItemsState(data);

		eventDropState(this, state);
	}

	void DDContainer::endDrop(bool _reset)
	{
		if (mStartDrop) {
			removeDropItems();

			// сбрасываем старую подсветку
			if (mDropInfo.reseiver) mDropInfo.reseiver->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

			if (_reset) mDropResult = false;
			eventEndDrop(this, mDropInfo, mDropResult);
			eventDropState(this, DROP_END);
			enableToolTip(true);

			// сбрасываем инфу для дропа
			mStartDrop = false;
			mDropResult = false;
			mOldDrop = null;
			mDropInfo.reset();
			mDropSenderIndex = ITEM_NONE;
		}
	}

	void DDContainer::removeDropItems()
	{
		for (VectorDropWidgetInfo::iterator iter=mDropItems.begin(); iter!=mDropItems.end(); ++iter) {
			iter->item->hide();
		}
		mDropItems.clear();
	}

	void DDContainer::updateDropItems()
	{
		if (mDropItems.empty()) {
			requestDropWidgetInfo(this, mDropItems);
		}

		const IntPoint & point = InputManager::getInstance().getMousePosition();

		for (VectorDropWidgetInfo::iterator iter=mDropItems.begin(); iter!=mDropItems.end(); ++iter) {
			iter->item->setPosition(point.left - mClickInWidget.left + iter->dimension.left, point.top - mClickInWidget.top + iter->dimension.top, iter->dimension.width, iter->dimension.height);
			iter->item->show();
		}
	}

	void DDContainer::updateDropItemsState(const DropWidgetState & _state)
	{
		eventUpdateDropState(this, mDropItems, _state);
	}

	void DDContainer::notifyInvalideDrop(WidgetPtr _sender)
	{
		mouseDrag();
	}

	void DDContainer::getContainer(WidgetPtr & _container, size_t & _index)
	{
		_container = this;
		_index = ITEM_NONE;
	}

} // namespace MyGUI
