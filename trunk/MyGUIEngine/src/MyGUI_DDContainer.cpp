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
		mNeedDrop(false),
		mStartDrop(false),
		mOldDrop(null),
		mCurrentSender(null),
		mDropSenderIndex(ITEM_NONE),
		mNeedDragDrop(false),
		mReseiverContainer(null)
	{
	}

	void DDContainer::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		// �������� ������ �������, ���� �������� ������
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
			// ���������� ���� ��� �����
			mDropResult = false;
			mOldDrop = null;
			mDropInfo.reset();
			mReseiverContainer = null;

			// ����������, ����� ��������� ������� ������
			mCurrentSender = null;
			mStartDrop = false;

		}
		// ���� ������ ������ ������� � ��� ���� �� ����������
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
		// ����� �� �������� ������
		bool update = false;

		// ������ ��� ������� �������
		if (false == mStartDrop) {
			mStartDrop = true;
			mNeedDrop = false;
			update = true;
			// ������ �� �������� ����� �� �������
			mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
			mReseiverContainer = null;
			eventStartDrop(this, mDropInfo, mNeedDrop);

			if (mNeedDrop) {
				eventDropState(this, DropItemState::Start);
				enableToolTip(false);
			}
			else {
				// ���������� ����� ���� (�� �����������)
				InputManager::getInstance().resetMouseCaptureWidget();
			}
		}

		// ���� �� �����
		if (false == mNeedDrop) {
			return;
		}

		// ������ ������, ��� ��� ���� ����
		const IntPoint & point = InputManager::getInstance().getMousePosition();
		WidgetPtr item = InputManager::getInstance().getWidgetFromPoint(point.left, point.top);

		updateDropItems();

		// ���� �����, ������ ��� ����������
		if (mOldDrop == item) return;
		mOldDrop = item;

		// ���������� ������ ���������
		if (mReseiverContainer) mReseiverContainer->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

		mDropResult = false;
		mReseiverContainer = null;
		WidgetPtr reseiver = null;
		size_t reseiver_index = ITEM_NONE;
		// ���� ������ ��� ����
		if (item) {
			// ������ ������ �� ������ �� ������������� �������
			item->getContainer(reseiver, reseiver_index);
			// �������� ������ � ������������
			if (reseiver && reseiver->isType<DDContainer>()) {
				// ������������� �� ���������� � ���������� �����
				mReseiverContainer = static_cast<DDContainerPtr>(reseiver);
				mReseiverContainer->eventInvalideContainer = newDelegate(this, &DDContainer::notifyInvalideDrop);

				// ������ ������ �� ����������� �����
				mDropInfo.set(this, mDropSenderIndex, reseiver, reseiver_index);
				eventRequestDrop(this, mDropInfo, mDropResult);

				// ������������� ����� ���������
				mReseiverContainer->setContainerItemInfo(mDropInfo.reseiver_index, true, mDropResult);
			}
			else {
				mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
			}
		}
		// ��� ������� ��� ����
		else {
			mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
		}

		DropItemState state;

		DropWidgetState data(mDropSenderIndex);
		data.update = update;

		if (reseiver == null) {
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

			// ���������� ������ ���������
			if (mReseiverContainer) mReseiverContainer->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

			if (_reset) mDropResult = false;
			eventEndDrop(this, mDropInfo, mDropResult);
			eventDropState(this, DropItemState::End);
			enableToolTip(true);

			// ���������� ���� ��� �����
			mStartDrop = false;
			mDropResult = false;
			mOldDrop = null;
			mDropInfo.reset();
			mReseiverContainer = null;
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
