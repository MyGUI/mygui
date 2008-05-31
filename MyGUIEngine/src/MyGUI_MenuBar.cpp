/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "MyGUI_MenuBar.h"
#include "MyGUI_Button.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{
	Ogre::String MenuBar::WidgetTypeName = "MenuBar";

	MenuBar::MenuBar(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mDistanceButton(0)
	{
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMouseButtonPressed);
			}
		}

		// парсим свойства
		const MapString & param = _info->getParams();
		if (false == param.empty()) {
			MapString::const_iterator iter = param.find("DistanceButton");
			if (iter != param.end()) mDistanceButton = utility::parseInt(iter->second);

			iter = param.find("SkinButton");
			if (iter != param.end()) mButtonSkinName = iter->second;
		}

		if (mWidgetClient == null) mWidgetClient= this;
	}

	void MenuBar::insertItem(size_t _index, const Ogre::UTFString & _item)
	{
		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size() || _index == ITEM_NONE, "index '" << _index << "' out of range");
		if (_index == ITEM_NONE) _index = mVectorMenuItemInfo.size();

		ButtonPtr button = mWidgetClient->createWidget<Button>(mButtonSkinName, IntCoord(), ALIGN_DEFAULT);
		button->setCaption(_item);

		mVectorMenuItemInfo.insert(mVectorMenuItemInfo.begin() + _index, MenuItemInfo(button));
		update();
	}

	void MenuBar::setItem(size_t _index, const Ogre::UTFString & _item)
	{
		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size(), "index '" << _index << "' out of range");
		mVectorMenuItemInfo[_index].button->setCaption(_item);

		update();
	}

	const Ogre::UTFString & MenuBar::getItem(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size(), "index '" << _index << "' out of range");
		return mVectorMenuItemInfo[_index].button->getCaption();
	}

	void MenuBar::deleteItem(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size(), "index '" << _index << "' out of range");

		WidgetManager::getInstance().destroyWidget(mVectorMenuItemInfo[_index].button);
		mVectorMenuItemInfo.erase(mVectorMenuItemInfo.begin() + _index);

		update();
	}

	void MenuBar::deleteAllItems()
	{
		for (VectorMenuItemInfo::iterator iter=mVectorMenuItemInfo.begin(); iter!=mVectorMenuItemInfo.end(); ++iter) {
			WidgetManager::getInstance().destroyWidget((*iter).button);
		}

		mVectorMenuItemInfo.clear();

		update();
	}

	void MenuBar::update()
	{
		int pos = 0;
		for (VectorMenuItemInfo::iterator iter=mVectorMenuItemInfo.begin(); iter!=mVectorMenuItemInfo.end(); ++iter) {
			int width = (*iter).button->getCoord().width - (*iter).button->getTextCoord().width + (*iter).button->getTextSize().width;
			(*iter).button->setPosition(pos, 0, width, mWidgetClient->getHeight());
			pos += width + mDistanceButton;
		}
	}

} // namespace MyGUI
