/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*//*
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
#include "MyGUI_Tab.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_Button.h"
#include "MyGUI_TabItem.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	const float TAB_SPEED_FADE_COEF = 5.0f;

	Tab::Tab(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mOffsetTab(0),
		mButtonShow(false),
		mWidthBar(0),
		mWidgetBar(nullptr),
		mButtonLeft(nullptr), mButtonRight(nullptr), mButtonList(nullptr),
		mButtonDecor(nullptr),
		mEmptyBarWidget(nullptr),
		mItemTemplate(nullptr),
		mStartIndex(0),
		mIndexSelect(ITEM_NONE),
		mButtonDefaultWidth(1),
		mSmoothShow(true),
		mButtonAutoWidth(true),
		mShutdown(false)
	{
		initialiseWidgetSkin(_info);
	}

	Tab::~Tab()
	{
		mShutdown = true;
		shutdownWidgetSkin();
	}

	void Tab::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void Tab::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// парсим свойства
		const MapString & properties = _info->getProperties();
		if (false == properties.empty()) {
			MapString::const_iterator iter = properties.find("OffsetBar");
			if (iter != properties.end()) mOffsetTab = utility::parseInt(iter->second);

			iter = properties.find("ButtonSkin");
			if (iter != properties.end()) mButtonSkinName = iter->second;
			iter = properties.find("EmptyBarSkin");
			if (iter != properties.end()) mEmptySkinName = iter->second;
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "Bar") {
				MYGUI_DEBUG_ASSERT( ! mWidgetBar, "widget already assigned");
				mWidgetBar = (*iter);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "Left") {
				MYGUI_DEBUG_ASSERT( ! mButtonLeft, "widget already assigned");
				mButtonLeft = (*iter)->castType<Button>();
				mButtonLeft->setVisible(false);
				mButtonLeft->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "Right") {
				MYGUI_DEBUG_ASSERT( ! mButtonRight, "widget already assigned");
				mButtonRight = (*iter)->castType<Button>();
				mButtonRight->setVisible(false);
				mButtonRight->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "List") {
				MYGUI_DEBUG_ASSERT( ! mButtonList, "widget already assigned");
				mButtonList = (*iter)->castType<Button>();
				mButtonList->setVisible(false);
				mButtonList->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "ButtonDecor") {
				MYGUI_DEBUG_ASSERT( ! mButtonDecor, "widget already assigned");
				mButtonDecor = *iter;
				mButtonDecor->setVisible(false);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "ShowPatch") {
				mWidgetsPatch.push_back((*iter));
				(*iter)->setVisible(false);
			}
			else if ((*(*iter)->_getInternalData<std::string>() == "Sheet") || (*(*iter)->_getInternalData<std::string>() == "TabItem")) {
				MYGUI_DEBUG_ASSERT( ! mItemTemplate, "widget already assigned");
				mItemTemplate = (*iter);
				mItemTemplate->setVisible(false);
			}
		}
		MYGUI_ASSERT(nullptr != mWidgetBar, "Child Widget Bar not found in skin (Tab must have Bar)");
		MYGUI_ASSERT(nullptr != mItemTemplate, "Child Widget TabItem not found in skin (Tab must have TabItem (Sheet) )");

		// создаем виджет, носитель скина пустоты бара
		mEmptyBarWidget = mWidgetBar->createWidget<Widget>(mEmptySkinName, IntCoord(), Align::Left | Align::Top);

		updateBar();
	}

	void Tab::shutdownWidgetSkin()
	{
		mWidgetsPatch.clear();
		mWidgetBar = nullptr;
		mButtonLeft = nullptr;
		mButtonRight = nullptr;
		mButtonList = nullptr;
		mButtonDecor = nullptr;
		mItemTemplate = nullptr;
		mEmptyBarWidget = nullptr;
	}


	// переопределяем для особого обслуживания страниц
	WidgetPtr Tab::baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		if ((TabItem::getClassTypeName() == _type) || ("Sheet" == _type)) {

			TabItemPtr sheet = static_cast<TabItemPtr>(Base::baseCreateWidget(_style, TabItem::getClassTypeName(), "Default", mItemTemplate->getCoord(), mItemTemplate->getAlign(), "", _name));
			_insertItem(ITEM_NONE, _name, sheet, Any::Null);

			return sheet;
		}
		return Base::baseCreateWidget(_style, _type, _skin, _coord, _align, _layer, _name);
	}

	TabItemPtr Tab::insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "Tab::insertItem");

		TabItemPtr sheet = static_cast<TabItemPtr>(Base::baseCreateWidget(WidgetStyle::Child, TabItem::getClassTypeName(), "Default", mItemTemplate->getCoord(), mItemTemplate->getAlign(), "", ""));
		_insertItem(_index, _name, sheet, _data);

		return sheet;
	}

	void Tab::setPosition(const IntPoint & _point)
	{
		Base::setPosition(_point);

		updateBar();
	}

	void Tab::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateBar();
	}

	void Tab::setCoord(const IntCoord & _coord)
	{
		Base::setCoord(_coord);

		updateBar();
	}

	void Tab::updateBar()
	{
		// подстраховка
		if (mWidgetBar->getWidth() < 1) return;

		if ((mWidgetBar->getWidth() < mWidthBar) && (1 < mItemsInfo.size())) {
			if (false == mButtonShow) {
				mButtonShow = true;
				if (nullptr != mButtonLeft) mButtonLeft->setVisible(true);
				if (nullptr != mButtonRight) mButtonRight->setVisible(true);
				if (nullptr != mButtonList) mButtonList->setVisible(true);
				if (nullptr != mButtonDecor) mButtonDecor->setVisible(true);
				for (VectorWidgetPtr::iterator iter=mWidgetsPatch.begin(); iter!=mWidgetsPatch.end(); ++iter) (*iter)->setVisible(true);
				mWidgetBar->setSize(mWidgetBar->getWidth() - mOffsetTab, mWidgetBar->getHeight());
			}
		}
		else {
			if (mButtonShow) {
				mButtonShow = false;
				if (nullptr != mButtonLeft) mButtonLeft->setVisible(false);
				if (nullptr != mButtonRight) mButtonRight->setVisible(false);
				if (nullptr != mButtonList) mButtonList->setVisible(false);
				if (nullptr != mButtonDecor) mButtonDecor->setVisible(false);
				for (VectorWidgetPtr::iterator iter=mWidgetsPatch.begin(); iter!=mWidgetsPatch.end(); ++iter) (*iter)->setVisible(false);
				mWidgetBar->setSize(mWidgetBar->getWidth() + mOffsetTab, mWidgetBar->getHeight());
			}
		}

		// проверяем правильность стартового индекса
		if (mStartIndex > 0) {
			// считаем длинну видимых кнопок
			int width = 0;
			for (size_t pos=mStartIndex; pos<mItemsInfo.size(); pos++) width += mItemsInfo[pos].width;

			// уменьшаем индекс до тех пор пока кнопка до индекста полностью не влезет в бар
			while ((mStartIndex > 0) && ((width + mItemsInfo[mStartIndex-1].width) <= mWidgetBar->getWidth())) {
				mStartIndex--;
				width += mItemsInfo[mStartIndex].width;
			}
		}

		// проверяем и обновляем бар
		int width = 0;
		size_t count = 0;
		size_t pos=mStartIndex;
		for (; pos<mItemsInfo.size(); pos++) {

			// текущая кнопка не влазиет
			if (width > mWidgetBar->getWidth()) break;

			// следующая не влазиет
			TabItemInfo & info = mItemsInfo[pos];
			if ((width + info.width) > mWidgetBar->getWidth()) {
				break;
			}

			// проверяем физическое наличие кнопки
			if (count >= mItemButton.size()) _createItemButton();

			// если кнопка не соответствует, то изменяем ее
			ButtonPtr button = mItemButton[count]->castType<Button>();
			button->setVisible(true);

			// корректируем нажатость кнопки
			button->setButtonPressed(pos == mIndexSelect);

			if (button->getCaption() != info.name)
				button->setCaption(info.name);
			// положение кнопки
			IntCoord coord(width, 0, info.width, mWidgetBar->getHeight());
			if (coord != button->getCoord())
				button->setCoord(coord);

			width += info.width;
			count ++;
		}

		// скрываем кнопки что были созданны, но не видны
		while (count < mItemButton.size()) {
			mItemButton[count]->setVisible(false);
			count ++;
		}

		bool right = true;
		if (pos == mItemsInfo.size()) right = false;

		// корректируем виджет для пустоты
		if (width < mWidgetBar->getWidth()) {
			mEmptyBarWidget->setVisible(true);
			mEmptyBarWidget->setCoord(width, 0, mWidgetBar->getWidth() - width, mWidgetBar->getHeight());
		}
		else {
			mEmptyBarWidget->setVisible(false);
		}

		// корректируем доступность стрелок
		if (mStartIndex == 0) {
			if (nullptr != mButtonLeft) mButtonLeft->setEnabled(false);
		}
		else {
			if (nullptr != mButtonLeft) mButtonLeft->setEnabled(true);
		}

		if (right) {
			if (nullptr != mButtonRight) mButtonRight->setEnabled(true);
		}
		else {
			if (nullptr != mButtonRight) mButtonRight->setEnabled(false);
		}

	}

	void Tab::notifyPressedButtonEvent(MyGUI::WidgetPtr _sender)
	{
		if (_sender == mButtonLeft) {
			if (mStartIndex > 0) {
				mStartIndex --;
				updateBar();
			}
		}
		else if (_sender == mButtonRight) {
			if ((mStartIndex+1) < mItemsInfo.size()) {
				mStartIndex ++;
				// в updateBar() будет подкорректированно если что
				updateBar();
			}
		}
		else if (_sender == mButtonList) {
		}
	}

	void Tab::notifyPressedBarButtonEvent(MyGUI::WidgetPtr _sender)
	{
		size_t select = *_sender->_getInternalData<size_t>() + mStartIndex;
		// щелкнули по той же кнопке
		if (select == mIndexSelect) {
			// стараемся показать выделенную кнопку
			beginToItemSelected();
			return;
		}
		size_t old = mIndexSelect;
		mIndexSelect = select;

		size_t count = 0;
		for (size_t pos=0; pos<mItemButton.size(); pos++) {
			ButtonPtr button = mItemButton[count]->castType<Button>();
			if (button->isVisible()) {
				// корректируем нажатость кнопки
				button->setButtonPressed((pos + mStartIndex) == mIndexSelect);
			}
			count ++;
		}

		// стараемся показать выделенную кнопку
		beginToItemSelected();

		// поднимаем страницу для пикинга
		_forcePeek(mItemsInfo[mIndexSelect].item);

		_showItem(mItemsInfo[mIndexSelect].item, true, mSmoothShow);
		_showItem(mItemsInfo[old].item, false, mSmoothShow);

		eventTabChangeSelect(this, mIndexSelect);
	}

	void Tab::beginToItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::beginToItemAt");

		// подстраховка
		if (mWidgetBar->getWidth() < 1) return;

		if (_index == mStartIndex) return;
		else if (_index < mStartIndex) {
			mStartIndex = _index;
			updateBar();
		}
		else {
			// длинна бара от старт индекса до нужной включительно
			int width = 0;
			for (size_t pos=mStartIndex; pos<=_index; pos++) {
				width += mItemsInfo[pos].width;
			}

			// уменьшем старт индекс пока не появиться нужная
			bool change = false;
			while ((mStartIndex < _index) && (width > mWidgetBar->getWidth())) {
				width -= mItemsInfo[mStartIndex].width;
				mStartIndex ++;
				change = true;
			}
			if (change) updateBar();

		}
	}

	void Tab::setButtonDefaultWidth(int _width)
	{
		mButtonDefaultWidth = _width;
		if (mButtonDefaultWidth < 1) mButtonDefaultWidth = 1;
		setButtonAutoWidth(false);
	}

	void Tab::setButtonAutoWidth(bool _auto)
	{
		mButtonAutoWidth = _auto;

		for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
			int width;
			if (mButtonAutoWidth) width = _getTextWidth(mItemsInfo[pos].name);
			else width = mButtonDefaultWidth;

			mWidthBar += width - mItemsInfo[pos].width;
			mItemsInfo[pos].width = width;
		}

		updateBar();
	}

	void Tab::setButtonWidthAt(size_t _index, int _width)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::setButtonWidthAt");

		if (_width <= 0) {
			if (mButtonAutoWidth) _width = _getTextWidth(mItemsInfo[_index].name);
			else _width = mButtonDefaultWidth;
		}

		mWidthBar += _width - mItemsInfo[_index].width;
		mItemsInfo[_index].width = _width;

		updateBar();
	}

	void Tab::setItemNameAt(size_t _index, const Ogre::UTFString& _name)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::setItemNameAt");
		mItemsInfo[_index].name = _name;

		int width;
		if (mButtonAutoWidth) width = _getTextWidth(_name);
		else width = mButtonDefaultWidth;

		mWidthBar += width - mItemsInfo[_index].width;
		mItemsInfo[_index].width = width;

		updateBar();
	}

	void Tab::setIndexSelected(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::setIndexSelected");
		if (mIndexSelect == _index) return;
		size_t old = mIndexSelect;
		mIndexSelect = _index;
		updateBar();

		// поднимаем страницу для пикинга
		if (mSmoothShow) _forcePeek(mItemsInfo[mIndexSelect].item);

		_showItem(mItemsInfo[mIndexSelect].item, true, mSmoothShow);
		_showItem(mItemsInfo[old].item, false, mSmoothShow);

		beginToItemSelected();
	}

	void Tab::actionWidgetHide(WidgetPtr _widget)
	{
		_widget->setVisible(false);
		_widget->setEnabled(true);
	}

	void Tab::_showItem(TabItemPtr _item, bool _show, bool _smooth)
	{
		if (false == _smooth) {
			ControllerManager::getInstance().removeItem(_item);
			_item->setAlpha(ALPHA_MAX);

			_item->setVisible(_show);

			return;
		}

		if (_show) {
			ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MAX, TAB_SPEED_FADE_COEF, true);
			ControllerManager::getInstance().addItem(_item, controller);
		}
		else {
			ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, TAB_SPEED_FADE_COEF, false);
			controller->eventPostAction = newDelegate(this, &Tab::actionWidgetHide);
			ControllerManager::getInstance().addItem(_item, controller);
		}
	}

	void Tab::_createItemButton()
	{
		ButtonPtr button = mWidgetBar->createWidget<Button>(mButtonSkinName, IntCoord(), Align::Left | Align::Top);
		button->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedBarButtonEvent);
		button->_setInternalData(mItemButton.size()); // порядковый номер
		mItemButton.push_back(button);
	}

	int Tab::_getTextWidth(const Ogre::UTFString& _text)
	{
		if (0 == mItemButton.size()) _createItemButton();

		Ogre::UTFString save = mItemButton[0]->getCaption();
		mItemButton[0]->setCaption(_text);

		ISubWidgetText* text = mItemButton[0]->getSubWidgetText();
		const IntSize& size = text ? text->getTextSize() : IntSize();
		const IntCoord& coord = text ? text->getCoord() : IntCoord();

		mItemButton[0]->setCaption(save);

		return size.width + mItemButton[0]->getWidth() - coord.width;
	}

	void Tab::_notifyDeleteItem(TabItemPtr _sheet)
	{
		// общий шутдаун виджета
		if (mShutdown) return;

		size_t index = getItemIndex(_sheet);

		mWidthBar -= mItemsInfo[index].width;
		mItemsInfo.erase(mItemsInfo.begin() + index);

		if (0 == mItemsInfo.size()) mIndexSelect = ITEM_NONE;
		else {
			if (index < mIndexSelect) mIndexSelect --;
			else if (index == mIndexSelect) {
				if (mIndexSelect == mItemsInfo.size()) mIndexSelect --;
				mItemsInfo[mIndexSelect].item->setVisible(true);
				mItemsInfo[mIndexSelect].item->setAlpha(ALPHA_MAX);
			}
		}

		updateBar();
	}

	void Tab::_insertItem(size_t _index, const Ogre::UTFString & _name, TabItemPtr _sheet, Any _data)
	{
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		// добавляем инфу о вкладке
		int width = (mButtonAutoWidth ? _getTextWidth(_name) : mButtonDefaultWidth);
		mWidthBar += width;

		mItemsInfo.insert(mItemsInfo.begin() + _index, TabItemInfo(width, _name, _sheet, _data));

		// первая вкладка
		if (1 == mItemsInfo.size()) mIndexSelect = 0;
		else {
			_sheet->setVisible(false);
			if (_index <= mIndexSelect) mIndexSelect ++;
		}

		updateBar();
	}

	void Tab::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::setItemDataAt");
		mItemsInfo[_index].data = _data;
	}

	int Tab::getButtonWidthAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::getButtonWidthAt");
		return mItemsInfo[_index].width;
	}

	const Ogre::UTFString & Tab::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::getItemNameAt");
		return mItemsInfo[_index].name;
	}

	TabItemPtr Tab::getItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::getItemAt");
		return mItemsInfo[_index].item;
	}

	void Tab::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::removeItemAt");
		this->_destroyChildWidget(mItemsInfo[_index].item);
	}

	void Tab::removeAllItems()
	{
		while (mItemsInfo.size() > 0) {
			this->_destroyChildWidget(mItemsInfo.back().item);
		}
	}

} // namespace MyGUI
