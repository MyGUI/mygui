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
			eventStartDrop(this, mDropInfo, mNeedDrop);

			if (mNeedDrop) {
				eventDropState(this, DROP_START);
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
		if (mDropInfo.reseiver) mDropInfo.reseiver->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

		mDropResult = false;
		WidgetPtr reseiver = null;
		size_t reseiver_index = ITEM_NONE;
		// ���� ������ ��� ����
		if (item) {
			// ������ ������ �� ������ �� ������������� �������
			item->getContainer(reseiver, reseiver_index);
			if (reseiver) {
				// ������������� �� ���������� � ���������� �����
				reseiver->eventInvalideContainer = newDelegate(this, &DDContainer::notifyInvalideDrop);

				// ������ ������ �� ����������� �����
				mDropInfo.set(this, mDropSenderIndex, reseiver, reseiver_index);
				eventRequestDrop(this, mDropInfo, mDropResult);

				// ������������� ����� ���������
				mDropInfo.reseiver->setContainerItemInfo(mDropInfo.reseiver_index, true, mDropResult);
			}
			else {
				mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
			}
		}
		// ��� ������� ��� ����
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

			// ���������� ������ ���������
			if (mDropInfo.reseiver) mDropInfo.reseiver->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

			if (_reset) mDropResult = false;
			eventEndDrop(this, mDropInfo, mDropResult);
			eventDropState(this, DROP_END);
			enableToolTip(true);

			// ���������� ���� ��� �����
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
