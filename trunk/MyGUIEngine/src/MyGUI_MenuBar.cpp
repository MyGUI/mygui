/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "MyGUI_MenuBar.h"
#include "MyGUI_Button.h"
#include "MyGUI_PopupMenu.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( MenuBar, Widget );

	MenuBar::MenuBar(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mDistanceButton(0),
		mIndexSelect(ITEM_NONE)
	{
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMouseButtonPressed);
			}
		}

		// парсим свойства
		const MapString & properties = _info->getProperties();
		if (false == properties.empty()) {
			MapString::const_iterator iter = properties.find("DistanceButton");
			if (iter != properties.end()) mDistanceButton = utility::parseInt(iter->second);

			iter = properties.find("SkinButton");
			if (iter != properties.end()) mButtonSkinName = iter->second;
			MYGUI_ASSERT(false == mButtonSkinName.empty(), "SkinButton property not found (MenuBar must have SkinButton property)");

			iter = properties.find("SubMenuSkin");
			if (iter != properties.end()) mSubMenuSkin = iter->second;
			MYGUI_ASSERT(false == mSubMenuSkin.empty(), "SubMenuSkin property not found (MenuBar must have SubMenuSkin property)");

			iter = properties.find("SubMenuLayer");
			if (iter != properties.end()) mSubMenuLayer = iter->second;
			MYGUI_ASSERT(false == mSubMenuLayer.empty(), "SubMenuLayer property not found (MenuBar must have SubMenuLayer property)");
		}

		if (mWidgetClient == null) mWidgetClient= this;
	}

	void MenuBar::insertItemAt(size_t _index, const Ogre::UTFString & _item, ItemType _type, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "MenuBar::insertItem");
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		ButtonPtr button = mWidgetClient->createWidget<Button>(mButtonSkinName, IntCoord(), Align::Default);
		button->eventMouseButtonPressed = newDelegate(this, &MenuBar::eventMouseButtonPressed);
		button->setCaption(_item);

		PopupMenuPtr menu = Gui::getInstance().createWidget<PopupMenu>(mSubMenuSkin, IntCoord(), Align::Default, mSubMenuLayer);
		//menu->eventPopupMenuClose = newDelegate(this, &MenuBar::notifyPopupMenuClose);
		menu->eventPopupMenuAccept = newDelegate(this, &MenuBar::notifyPopupMenuAccept);
		menu->_setOwner(this);

		mItemsInfo.insert(mItemsInfo.begin() + _index, MenuItemInfo(button, false, menu, _data));

		update();
	}

	void MenuBar::setItemNameAt(size_t _index, const Ogre::UTFString & _item)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuBar::setItemNameAt");
		mItemsInfo[_index].button->setCaption(_item);

		update();
	}

	const Ogre::UTFString & MenuBar::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuBar::getItemNameAt");
		return mItemsInfo[_index].button->getCaption();
	}

	PopupMenuPtr MenuBar::getItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuBar::getItemChildAt");
		return mItemsInfo[_index].menu;
	}

	void MenuBar::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuBar::removeItemAt");

		WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].button);
		WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].menu);
		mItemsInfo.erase(mItemsInfo.begin() + _index);

		update();
	}

	void MenuBar::removeAllItems()
	{
		for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
			WidgetManager::getInstance().destroyWidget((*iter).button);
			WidgetManager::getInstance().destroyWidget((*iter).menu);
		}

		mItemsInfo.clear();

		update();
	}

	void MenuBar::update()
	{
		int pos = 0;
		for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
			int width = (*iter).button->getCoord().width - (*iter).button->getTextCoord().width + (*iter).button->getTextSize().width;
			(*iter).button->setCoord(pos, 0, width, mWidgetClient->getHeight());
			pos += width + mDistanceButton;
			(*iter).button->_setInternalData((size_t)(iter - mItemsInfo.begin()));
		}
	}

	void MenuBar::eventMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		if (_id == MB_Left) {
			size_t select = *_sender->_getInternalData<size_t>();
			if (mIndexSelect == select) select = ITEM_NONE;
			setItemSelectedAt(select);
		}
	}

	void MenuBar::setItemSelectedAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mItemsInfo.size(), "MenuBar::setItemSelectedAt");

		if (mIndexSelect == _index) return;

		if (mIndexSelect != ITEM_NONE) {
			mItemsInfo[mIndexSelect].button->setButtonPressed(false);
			mItemsInfo[mIndexSelect].menu->hidePopupMenu(false);
		}

		mIndexSelect = _index;

		if (mIndexSelect != ITEM_NONE) {
			if (0 != mItemsInfo[mIndexSelect].menu->getItemCount()) {
				mItemsInfo[mIndexSelect].button->setButtonPressed(true);
				mItemsInfo[mIndexSelect].menu->showPopupMenu(
					IntPoint(int(mItemsInfo[mIndexSelect].button->getAbsoluteLeft()), getAbsoluteRect().bottom));
			}
			else {
				mIndexSelect = ITEM_NONE;
			}
		}
	}

	/*void MenuBar::notifyPopupMenuClose(WidgetPtr _sender)
	{
		// при щелчке на активный понкт меню сбрасывать не нужно
		if (mIndexSelect != ITEM_NONE) {
			const IntPoint & point = InputManager::getInstance().getMousePosition();
			WidgetPtr widget = InputManager::getInstance().getWidgetFromPoint(point.left, point.top);
			if (mItemsInfo[mIndexSelect].button == widget) return;
		}
		resetItemSelect();
	}*/

	void MenuBar::notifyPopupMenuAccept(WidgetPtr _sender, size_t _index)
	{
		//resetItemSelect();
		eventPopupMenuAccept(this, _sender->castType<PopupMenu>(), _index);
	}

	void MenuBar::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuBar::setItemDataAt");
		mItemsInfo[_index].data = _data;
	}

	PopupMenuPtr MenuBar::createItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuBar::createItemChildAt");

		if (mItemsInfo[_index].menu == null) {
			mItemsInfo[_index].menu = Gui::getInstance().createWidget<PopupMenu>(mSubMenuSkin, IntCoord(), Align::Default, mSubMenuLayer);
			mItemsInfo[_index].menu->_setOwner(this);
		}
		else {
			mItemsInfo[_index].menu->removeAllItems();
		}

		update();

		return mItemsInfo[_index].menu;
	}

	void MenuBar::removeItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuBar::removeItemChildAt");

		if (mItemsInfo[_index].menu != null) {
			WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].menu);
			mItemsInfo[_index].menu = null;
		}

		update();
	}

	/*MenuBar::ItemType MenuBar::getItemTypeAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuBar::getItemTypeAt");
		return getItemType(mItemsInfo[_index].submenu != null, mItemsInfo[_index].separator);
	}*/

} // namespace MyGUI
