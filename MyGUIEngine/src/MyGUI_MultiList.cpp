/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#include "MyGUI_MultiList.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Button.h"
#include "MyGUI_List.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	MultiList::MultiList(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mHeightButton(0),
		mWidgetClient(null),
		mWidthBar(0)
	{
		// парсим свойства
		const MapString & param = _info->getParams();
		if (false == param.empty()) {
			MapString::const_iterator iter = param.find("SkinButton");
			if (iter != param.end()) mSkinButton = iter->second;
			iter = param.find("HeightButton");
			if (iter != param.end()) mHeightButton = utility::parseInt(iter->second);
			if (mHeightButton < 0) mHeightButton = 0;

			iter = param.find("SkinList");
			if (iter != param.end()) mSkinList = iter->second;
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
		}
		// мона и без клиента
		if (null == mWidgetClient) mWidgetClient = this;

		mButtonMain = mWidgetClient->createWidget<Button>(mSkinButton, IntCoord(0, 0, mWidgetClient->getWidth(), mHeightButton), ALIGN_DEFAULT);
		//mMainList = mWidgetClient->createWidget<List>(mSkinButton, IntCoord(0, mHeightButton, mWidgetClient->getWidth(), mWidgetClient->getHeight() - mHeightButton), ALIGN_LEFT | ALIGN_VSTRETCH);

	}

	void MultiList::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
	}

	void MultiList::setPosition(const IntCoord& _coord)
	{
		Widget::setPosition(_coord);
	}

	void MultiList::insertRow(size_t _index, int _width, const Ogre::DisplayString & _name)
	{
		RowInfo row;
		row.width = _width < 0 ? 0 : _width;

		row.list = mWidgetClient->createWidget<List>(mSkinList, IntCoord(), ALIGN_LEFT | ALIGN_VSTRETCH);
		row.list->eventListChangePosition = newDelegate(this, &MultiList::notifyListChangePosition);

		row.button = mWidgetClient->createWidget<Button>(mSkinButton, IntCoord(), ALIGN_DEFAULT);
		row.button->setCaption(_name);

		if (_index > mVectorRowInfo.size()) _index = mVectorRowInfo.size();
		mVectorRowInfo.insert(mVectorRowInfo.begin() + _index, row);

		updateRows();
	}

	void MultiList::setRowName(size_t _index, const Ogre::DisplayString & _name)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		mVectorRowInfo[_index].button->setCaption(_name);
	}

	void MultiList::setRowWidth(size_t _index, int _width)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		mVectorRowInfo[_index].width = _width < 0 ? 0 : _width;

		updateRows();
	}

	const Ogre::DisplayString & MultiList::getRowName(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		return mVectorRowInfo[_index].button->getCaption();
	}

	int MultiList::getRowWidth(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		return mVectorRowInfo[_index].width;
	}

	void MultiList::deleteRow(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");

		RowInfo & info = mVectorRowInfo[_index];
		Gui & gui = Gui::getInstance();
		gui.destroyWidget(info.button);
		gui.destroyWidget(info.list);

		mVectorRowInfo.erase(mVectorRowInfo.begin() + _index);

		updateRows();
	}

	void MultiList::deleteAllRows()
	{
		Gui & gui = Gui::getInstance();
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			gui.destroyWidget((*iter).button);
			gui.destroyWidget((*iter).list);
		}
		mVectorRowInfo.clear();

		updateRows();
	}

	void MultiList::updateRows()
	{
		mWidthBar = 0;
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).button->setPosition(mWidthBar, 0, (*iter).width, mHeightButton);
			(*iter).list->setPosition(mWidthBar, mHeightButton, (*iter).width, mWidgetClient->getHeight() - mHeightButton);

			mWidthBar += (*iter).width;
		}

		// кнопка, для заполнения пустоты
		if (mWidthBar >= mWidgetClient->getWidth()) mButtonMain->hide();
		else {
			mButtonMain->setPosition(mWidthBar, 0, mWidgetClient->getWidth()-mWidthBar, mHeightButton);
			mButtonMain->show();
		}
	}

	void MultiList::updateOnlyEmpty()
	{
		// кнопка, для заполнения пустоты
		if (mWidthBar >= mWidgetClient->getWidth()) mButtonMain->hide();
		else {
			mButtonMain->setPosition(mWidthBar, 0, mWidgetClient->getWidth()-mWidthBar, mHeightButton);
			mButtonMain->show();
		}
	}


	size_t MultiList::getItemCount()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		return mVectorRowInfo.front().list->getItemCount();
	}

	void MultiList::insertItem(size_t _index, const Ogre::DisplayString & _item)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		if (_index > mVectorRowInfo.front().list->getItemCount()) _index = mVectorRowInfo.front().list->getItemCount();
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter)
			(*iter).list->insertItem(_index, "");
		mVectorRowInfo.front().list->setItem(_index, _item);
	}

	void MultiList::setItem(size_t _index, const Ogre::DisplayString & _item)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		mVectorRowInfo.front().list->setItem(_index, _item);
	}

	const Ogre::DisplayString & MultiList::getItem(size_t _index)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		return mVectorRowInfo.front().list->getItem(_index);
	}

	void MultiList::deleteItem(size_t _index)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter)
			(*iter).list->deleteItem(_index);
	}

	void MultiList::deleteAllItems()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter)
			(*iter).list->deleteAllItems();
	}

	size_t MultiList::getItemSelect()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		return mVectorRowInfo.front().list->getItemSelect();
	}

	void MultiList::resetItemSelect()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter)
			(*iter).list->resetItemSelect();
	}

	void MultiList::setItemSelect(size_t _index)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter)
			(*iter).list->setItemSelect(_index);
	}

	// саб строки
	void MultiList::setSubItem(size_t _row, size_t _index, const Ogre::DisplayString & _item)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		mVectorRowInfo[_row].list->setItem(_index, _item);
	}

	const Ogre::DisplayString & MultiList::getSubItem(size_t _row, size_t _index)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		return mVectorRowInfo[_row].list->getItem(_index);
	}

	void MultiList::notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter)
			(*iter).list->setItemSelect(_position);
	}

} // namespace MyGUI
