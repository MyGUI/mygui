/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#include "MyGUI_MultiList.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Button.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_List.h"
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( MultiList, Widget );

	MultiList::MultiList(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mHeightButton(0),
		mButtonMain(null),
		mWidthBar(0),
		mLastMouseFocusIndex(ITEM_NONE),
		mSortUp(true),
		mSortColumnIndex(ITEM_NONE),
		mIsDirtySort(false),
		mWidthSeparator(0),
		mOffsetButtonSeparator(2)
	{
		// ������ ��������
		const MapString & properties = _info->getProperties();
		if (false == properties.empty()) {
			MapString::const_iterator iter = properties.find("SkinButton");
			if (iter != properties.end()) mSkinButton = iter->second;
			iter = properties.find("HeightButton");
			if (iter != properties.end()) mHeightButton = utility::parseInt(iter->second);
			if (mHeightButton < 0) mHeightButton = 0;

			iter = properties.find("SkinList");
			if (iter != properties.end()) mSkinList = iter->second;

			iter = properties.find("SkinButtonEmpty");
			if (iter != properties.end()) {
				mButtonMain = mWidgetClient->createWidget<Button>(iter->second,
					IntCoord(0, 0, mWidgetClient->getWidth(), mHeightButton), Align::Default);
			}

			iter = properties.find("WidthSeparator");
			if (iter != properties.end()) mWidthSeparator = utility::parseInt(iter->second);
			iter = properties.find("SkinSeparator");
			if (iter != properties.end()) mSkinSeparator = iter->second;
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
			}
		}
		// ���� � ��� �������
		if (null == mWidgetClient) mWidgetClient = this;
	}

	MultiList::~MultiList()
	{
		Gui::getInstance().eventFrameStart -= newDelegate(this, &MultiList::frameEntered);
	}

	//----------------------------------------------------------------------------------//
	// ������ ��� ������ �� ���������
	void MultiList::insertColumnAt(size_t _column, const Ogre::UTFString & _name, int _width, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_column, mVectorColumnInfo.size(), "MultiList::insertColumnAt");
		if (_column == ITEM_NONE) _column = mVectorColumnInfo.size();

		// �������� � �������� ������
		if (false == mVectorColumnInfo.empty()) 
			mVectorColumnInfo.back().list->setScrollVisible(false);
		else mSortColumnIndex = 0;

		ColumnInfo column;
		column.width = _width < 0 ? 0 : _width;

		column.list = mWidgetClient->createWidget<List>(mSkinList, IntCoord(), Align::Left | Align::VStretch);
		column.list->eventListChangePosition = newDelegate(this, &MultiList::notifyListChangePosition);
		column.list->eventListMouseItemFocus = newDelegate(this, &MultiList::notifyListChangeFocus);
		column.list->eventListChangeScroll = newDelegate(this, &MultiList::notifyListChangeScrollPosition);
		column.list->eventListSelectAccept = newDelegate(this, &MultiList::notifyListSelectAccept);

		column.button = mWidgetClient->createWidget<Button>(mSkinButton, IntCoord(), Align::Default);
		column.button->eventMouseButtonClick = newDelegate(this, &MultiList::notifyButtonClick);
		column.name = _name;
		column.data = _data;

		// ���� ��� ���� ��������, �� ������ �� �� ����������� �����
		if (false == mVectorColumnInfo.empty()) {
			size_t count = mVectorColumnInfo.front().list->getItemCount();
			for (size_t pos=0; pos<count; ++pos)
				column.list->addItem("");
		}

		mVectorColumnInfo.insert(mVectorColumnInfo.begin() + _column, column);

		updateColumns();

		// ���������� ������ ������ ��������
		mVectorColumnInfo.back().list->setScrollVisible(true);
	}

	void MultiList::setColumnNameAt(size_t _column, const Ogre::UTFString & _name)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiList::setColumnNameAt");
		mVectorColumnInfo[_column].name = _name;
		redrawButtons();
	}

	void MultiList::setColumnWidthAt(size_t _column, int _width)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiList::setColumnWidthAt");
		mVectorColumnInfo[_column].width = _width < 0 ? 0 : _width;
		updateColumns();
	}

	const Ogre::UTFString & MultiList::getColumnNameAt(size_t _column)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiList::getColumnNameAt");
		return mVectorColumnInfo[_column].name;
	}

	int MultiList::getColumnWidthAt(size_t _column)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiList::getColumnWidthAt");
		return mVectorColumnInfo[_column].width;
	}

	void MultiList::removeColumnAt(size_t _column)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiList::removeColumnAt");

		ColumnInfo & info = mVectorColumnInfo[_column];

		WidgetManager & manager = WidgetManager::getInstance();
		manager.destroyWidget(info.button);
		manager.destroyWidget(info.list);

		mVectorColumnInfo.erase(mVectorColumnInfo.begin() + _column);

		if (mVectorColumnInfo.empty()) mSortColumnIndex = ITEM_NONE;
		else {
			mSortColumnIndex = 0;
			mSortUp = true;
			sortList();
		}

		updateColumns();
	}

	void MultiList::removeAllColumns()
	{
		WidgetManager & manager = WidgetManager::getInstance();
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			manager.destroyWidget((*iter).button);
			manager.destroyWidget((*iter).list);
		}
		mVectorColumnInfo.clear();
		mSortColumnIndex = ITEM_NONE;

		updateColumns();
	}

	void MultiList::sortByColumn(size_t _column, bool _backward)
	{
		mSortColumnIndex = _column;
		if (_backward) {
			mSortUp = !mSortUp;
			redrawButtons();
			// ���� ���� ���������������� �� ���������
			if (mIsDirtySort) sortList();

			flipList();
		}
		else {
			mSortUp = true;
			redrawButtons();
			sortList();
		}
	}

	size_t MultiList::getItemCount()
	{
		if (mVectorColumnInfo.empty()) return 0;
		return mVectorColumnInfo.front().list->getItemCount();
	}

	void MultiList::removeAllItems()
	{
		BiIndexBase::removeAllItems();
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->removeAllItems();
		}
	}

	size_t MultiList::getItemIndexSelected()
	{
		if (mVectorColumnInfo.empty()) return ITEM_NONE;
		size_t item = mVectorColumnInfo.front().list->getItemIndexSelected();
		return (ITEM_NONE == item) ? ITEM_NONE : BiIndexBase::convertToFace(item);
	}

	void MultiList::setItemSelectedAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(0, mVectorColumnInfo.size(), "MultiList::setItemSelectedAt");
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiList::setItemSelectedAt");

		if (_index == ITEM_NONE) {
			for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
				(*iter).list->clearItemSelected();
			}
		}
		else {
			size_t index = BiIndexBase::convertToBack(_index);
			for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
				(*iter).list->setItemSelectedAt(index);
			}
		}
	}

	void MultiList::setSubItemNameAt(size_t _column, size_t _index, const Ogre::UTFString & _name)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiList::setSubItemAt");
		MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiList::setSubItemAt");


		size_t index = BiIndexBase::convertToBack(_index);
		mVectorColumnInfo[_column].list->setItemNameAt(index, _name);

		// ���� �� ��������� ������ � �������� ������, ���� �������������
		if (_column == mSortColumnIndex) setDirtySort();
	}

	const Ogre::UTFString & MultiList::getSubItemNameAt(size_t _column, size_t _index)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiList::getSubItemNameAt");
		MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiList::getSubItemNameAt");

		size_t index = BiIndexBase::convertToBack(_index);
		return mVectorColumnInfo[_column].list->getItemNameAt(index);
	}

	size_t MultiList::findSubItemWith(size_t _column, const Ogre::UTFString & _name)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiList::findSubItemWith");

		size_t index = mVectorColumnInfo[_column].list->findItemIndexWith(_name);
		return index == ITEM_NONE ? ITEM_NONE : BiIndexBase::convertToFace(index);
	}
	//----------------------------------------------------------------------------------//

	void MultiList::updateOnlyEmpty()
	{
		if (null == mButtonMain) return;
		// ������, ��� ���������� �������
		if (mWidthBar >= mWidgetClient->getWidth()) mButtonMain->hide();
		else {
			mButtonMain->setCoord(mWidthBar, 0, mWidgetClient->getWidth()-mWidthBar, mHeightButton);
			mButtonMain->show();
		}
	}

	void MultiList::notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter)
			if (_widget != (*iter).list) (*iter).list->setItemSelectedAt(_position);

		// ��� �����
		eventListChangePosition(this, _position);
	}

	void MultiList::notifyListSelectAccept(MyGUI::WidgetPtr _widget, size_t _position)
	{
		// ��� �����
		eventListSelectAccept(this, _position);
	}

	void MultiList::notifyListChangeFocus(MyGUI::WidgetPtr _widget, size_t _position)
	{
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			if (_widget != (*iter).list) {
				if (ITEM_NONE != mLastMouseFocusIndex) (*iter).list->_setItemFocus(mLastMouseFocusIndex, false);
				if (ITEM_NONE != _position) (*iter).list->_setItemFocus(_position, true);
			}
		}
		mLastMouseFocusIndex = _position;
	}

	void MultiList::notifyListChangeScrollPosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			if (_widget != (*iter).list)
				(*iter).list->setScrollPosition(_position);
		}
	}

	void MultiList::notifyButtonClick(MyGUI::WidgetPtr _widget)
	{
		size_t index = *_widget->_getInternalData<size_t>();
		sortByColumn(index, index == mSortColumnIndex);
	}

	void MultiList::redrawButtons()
	{
		size_t pos = 0;
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			if (pos == mSortColumnIndex) {
				if (mSortUp) setButtonImageIndex((*iter).button, SORT_UP);
				else setButtonImageIndex((*iter).button, SORT_DOWN);
			}
			else setButtonImageIndex((*iter).button, SORT_NONE);
			(*iter).button->setCaption((*iter).name);
			pos++;
		}
	}

	void MultiList::setButtonImageIndex(ButtonPtr _button, size_t _index)
	{
		StaticImagePtr image = _button->getStaticImage();
		if ( null == image ) return;
		if (image->getItemResource()) {
			static const int CountIcons = 3;
			static const char * IconNames[CountIcons + 1] = {"None", "Up", "Down", ""};
			if (_index >= CountIcons) _index = CountIcons;
			image->setItemName(IconNames[_index]);
		}
		else {
			image->setItemSelect(_index);
		}
	}

	void MultiList::setDirtySort()
	{
		if (mIsDirtySort) return;
		Gui::getInstance().eventFrameStart += newDelegate(this, &MultiList::frameEntered);
		mIsDirtySort = true;
	}

	void MultiList::frameEntered(float _frame)
	{
		if (false == mIsDirtySort) {
			Gui::getInstance().eventFrameStart -= newDelegate(this, &MultiList::frameEntered);
			return;
		}
		sortList();
	}

	WidgetPtr MultiList::getSeparator(size_t _index)
	{
		if (!mWidthSeparator || mSkinSeparator.empty()) return null;
		// ��������� �������
		if (_index == mVectorColumnInfo.size()-1) return null;

		while (_index >= mSeparators.size()) {
			WidgetPtr separator = mWidgetClient->createWidget<Widget>(mSkinSeparator, IntCoord(), Align::Default);
			mSeparators.push_back(separator);
		}

		return mSeparators[_index];
	}

	void MultiList::updateColumns()
	{
		mWidthBar = 0;
		size_t index = 0;
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->setCoord(mWidthBar, mHeightButton, (*iter).width, mWidgetClient->getHeight() - mHeightButton);
			(*iter).button->setCoord(mWidthBar, 0, (*iter).width, mHeightButton);
			(*iter).button->_setInternalData(index);

			mWidthBar += (*iter).width;

			// ���������� ����� �������
			WidgetPtr separator = getSeparator(index);
			if (separator) {
				separator->setCoord(mWidthBar, 0, mWidthSeparator, mWidgetClient->getHeight());
			}

			mWidthBar += mWidthSeparator;
			index++;
		}

		redrawButtons();
		updateOnlyEmpty();
	}

	void MultiList::flipList()
	{
		if (ITEM_NONE == mSortColumnIndex) return;

		size_t last = mVectorColumnInfo.front().list->getItemCount();
		if (0 == last) return;
		last --;
		size_t first = 0;

		while (first < last) {

			BiIndexBase::swapItemsBackAt(first, last);
			for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
				(*iter).list->swapItemsAt(first, last);
			}

			first++;
			last--;
		}
	}

	bool MultiList::compare(ListPtr _list, size_t _left, size_t _right)
	{
		bool result;
		if(mSortUp) std::swap(_left, _right);
		if (operatorLess.empty()) result = _list->getItemNameAt(_left) < _list->getItemNameAt(_right);
		else operatorLess(this, mSortColumnIndex, _list->getItemNameAt(_left), _list->getItemNameAt(_right), result);
		return result;
	}

	void MultiList::sortList()
	{
		if (ITEM_NONE == mSortColumnIndex) return;

		ListPtr list = mVectorColumnInfo[mSortColumnIndex].list;

		size_t count = list->getItemCount();
		if (0 == count) return;

		// shell sort
		int first, last;
		for (size_t step = count>>1; step>0 ; step >>= 1) {
			for (size_t i=0;i<(count-step);i++)
			{
				first=i;
				while (first>=0) 
				{
					last = first+step;
					if (compare(list, first, last))
					{
						BiIndexBase::swapItemsBackAt(first, last);
						for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
							(*iter).list->swapItemsAt(first, last);
						}
					}
					first--;
				}
			}
		}


		mIsDirtySort = false;
	}

	void MultiList::insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data)
	{
		MYGUI_ASSERT_RANGE(0, mVectorColumnInfo.size(), "MultiList::insertItemAt");
		MYGUI_ASSERT_RANGE_INSERT(_index, mVectorColumnInfo.front().list->getItemCount(), "MultiList::insertItemAt");
		if (ITEM_NONE == _index) _index = mVectorColumnInfo.front().list->getItemCount();

		size_t index = BiIndexBase::insertItemAt(_index);

		// ��������� �� ��� ���� ������, � ����� ����������� �������
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->insertItemAt(index, "");
		}
		mVectorColumnInfo.front().list->setItemNameAt(index, _name);
		mVectorColumnInfo.front().list->setItemDataAt(index, _data);

		setDirtySort();
	}

	void MultiList::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(0, mVectorColumnInfo.size(), "MultiList::removeItemAt");
		MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiList::removeItemAt");

		size_t index = BiIndexBase::removeItemAt(_index);

		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->removeItemAt(index);
		}
	}

	void MultiList::swapItemsAt(size_t _index1, size_t _index2)
	{
		MYGUI_ASSERT_RANGE(0, mVectorColumnInfo.size(), "MultiList::removeItemAt");
		MYGUI_ASSERT_RANGE(_index1, mVectorColumnInfo.begin()->list->getItemCount(), "MultiList::swapItemsAt");
		MYGUI_ASSERT_RANGE(_index2, mVectorColumnInfo.begin()->list->getItemCount(), "MultiList::swapItemsAt");

		// ��� �������������, ������ ������ �������
		BiIndexBase::swapItemsFaceAt(_index1, _index2);

		// ��� ���������������, ����� ��������, �������� ������ ������
		// FIXME
	}

} // namespace MyGUI
