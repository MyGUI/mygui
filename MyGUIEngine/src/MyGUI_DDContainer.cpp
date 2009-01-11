/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	DDContainer::DDContainer(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Widget(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mDropResult(false),
		mNeedDrop(false),
		mStartDrop(false),
		mOldDrop(nullptr),
		mCurrentSender(nullptr),
		mDropSenderIndex(ITEM_NONE),
		mNeedDragDrop(false),
		mReseiverContainer(nullptr)
	{
		initialiseWidgetSkin(_info);
	}

	DDContainer::~DDContainer()
	{
		shutdownWidgetSkin();
	}

	void DDContainer::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void DDContainer::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
	}

	void DDContainer::shutdownWidgetSkin()
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
		if (MouseButton::Left == _id) {
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
		else {
			endDrop(true);
		}
	}

	void DDContainer::mouseButtonReleased(MouseButton _id)
	{
		if (MouseButton::Left == _id) {
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
			mDropInfo.set(this, mDropSenderIndex, nullptr, ITEM_NONE);
			mReseiverContainer = nullptr;
			eventStartDrop(this, mDropInfo, mNeedDrop);

			if (mNeedDrop) {
				eventDropState(this, DropItemState::Start);
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
		WidgetPtr item = LayerManager::getInstance().getWidgetFromPoint(point.left, point.top);

		updateDropItems();

		// если равно, значит уже спрашивали
		if (mOldDrop == item) return;
		mOldDrop = item;

		// сбрасываем старую подсветку
		if (mReseiverContainer) mReseiverContainer->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

		mDropResult = false;
		mReseiverContainer = nullptr;
		WidgetPtr reseiver = nullptr;
		size_t reseiver_index = ITEM_NONE;
		// есть виджет под нами
		if (item) {
			// делаем запрос на индекс по произвольному виджету
			item->getContainer(reseiver, reseiver_index);
			// работаем только с контейнерами
			if (reseiver && reseiver->isType<DDContainer>()) {
				// подписываемся на информацию о валидности дропа
				mReseiverContainer = static_cast<DDContainerPtr>(reseiver);
				mReseiverContainer->eventInvalideContainer = newDelegate(this, &DDContainer::notifyInvalideDrop);

				// делаем запрос на возможность дропа
				mDropInfo.set(this, mDropSenderIndex, reseiver, reseiver_index);
				eventRequestDrop(this, mDropInfo, mDropResult);

				// устанавливаем новую подсветку
				mReseiverContainer->setContainerItemInfo(mDropInfo.reseiver_index, true, mDropResult);
			}
			else {
				mDropInfo.set(this, mDropSenderIndex, nullptr, ITEM_NONE);
			}
		}
		// нет виджета под нами
		else {
			mDropInfo.set(this, mDropSenderIndex, nullptr, ITEM_NONE);
		}

		DropItemState state;

		DropWidgetState data(mDropSenderIndex);
		data.update = update;

		if (reseiver == nullptr) {
			data.accept = false;
			data.refuse = false;
			state = DropItemState::Miss;
		}
		else if (mDropResult) {
			data.accept = true;
			data.refuse = false;
			state = DropItemState::Accept;
		}
		else {
			data.accept = false;
			data.refuse = true;
			state = DropItemState::Refuse;
		}

		updateDropItemsState(data);

		eventDropState(this, state);
	}

	void DDContainer::endDrop(bool _reset)
	{
		if (mStartDrop) {
			removeDropItems();

			// сбрасываем старую подсветку
			if (mReseiverContainer) mReseiverContainer->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

			if (_reset) mDropResult = false;
			eventEndDrop(this, mDropInfo, mDropResult);
			eventDropState(this, DropItemState::End);
			enableToolTip(true);

			// сбрасываем инфу для дропа
			mStartDrop = false;
			mDropResult = false;
			mOldDrop = nullptr;
			mDropInfo.reset();
			mReseiverContainer = nullptr;
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
			iter->item->setCoord(point.left - mClickInWidget.left + iter->dimension.left, point.top - mClickInWidget.top + iter->dimension.top, iter->dimension.width, iter->dimension.height);
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
