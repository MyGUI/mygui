/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Tab.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_Button.h"
#include "MyGUI_TabItem.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( Tab, Widget );

	const float TAB_SPEED_FADE_COEF = 5.0f;

	Tab::Tab(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Widget(_coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mOffsetTab(0),
		mButtonShow(false),
		mWidthBar(0),
		mWidgetBar(null),
		mButtonLeft(null), mButtonRight(null), mButtonList(null),
		mButtonDecor(null),
		mEmptyBarWidget(null),
		mItemTemplate(null),
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
		Widget::baseChangeWidgetSkin(_info);
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
				mButtonLeft->hide();
				mButtonLeft->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "Right") {
				MYGUI_DEBUG_ASSERT( ! mButtonRight, "widget already assigned");
				mButtonRight = (*iter)->castType<Button>();
				mButtonRight->hide();
				mButtonRight->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "List") {
				MYGUI_DEBUG_ASSERT( ! mButtonList, "widget already assigned");
				mButtonList = (*iter)->castType<Button>();
				mButtonList->hide();
				mButtonList->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "ButtonDecor") {
				MYGUI_DEBUG_ASSERT( ! mButtonDecor, "widget already assigned");
				mButtonDecor = *iter;
				mButtonDecor->hide();
			}
			else if (*(*iter)->_getInternalData<std::string>() == "ShowPatch") {
				mWidgetsPatch.push_back((*iter));
				(*iter)->hide();
			}
			else if ((*(*iter)->_getInternalData<std::string>() == "Sheet") || (*(*iter)->_getInternalData<std::string>() == "TabItem")) {
				MYGUI_DEBUG_ASSERT( ! mItemTemplate, "widget already assigned");
				mItemTemplate = (*iter);
				mItemTemplate->hide();
			}
		}
		MYGUI_ASSERT(null != mWidgetBar, "Child Widget Bar not found in skin (Tab must have Bar)");
		MYGUI_ASSERT(null != mItemTemplate, "Child Widget TabItem not found in skin (Tab must have TabItem (Sheet) )");

		// создаем виджет, носитель скина пустоты бара
		mEmptyBarWidget = mWidgetBar->createWidget<Widget>(mEmptySkinName, IntCoord(), Align::Left | Align::Top);

		updateBar();
	}

	void Tab::shutdownWidgetSkin()
	{
		mWidgetsPatch.clear();
		mWidgetBar = null;
		mButtonLeft = null;
		mButtonRight = null;
		mButtonList = null;
		mButtonDecor = null;
		mItemTemplate = null;
		mEmptyBarWidget = null;
	}


	// переопределяем для особого обслуживания страниц
	WidgetPtr Tab::baseCreateWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		if ((TabItem::getClassTypeName() == _type) || ("Sheet" == _type)) {

			TabItemPtr sheet = static_cast<TabItemPtr>(Widget::baseCreateWidget(TabItem::getClassTypeName(), "Default", mItemTemplate->getCoord(), mItemTemplate->getAlign(), "", ""));
			_insertItem(ITEM_NONE, _name, sheet, Any::Null);

			return sheet;
		}
		return Widget::baseCreateWidget(_type, _skin, _coord, _align, _layer, _name);
	}

	TabItemPtr Tab::insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "Tab::insertItem");

		TabItemPtr sheet = static_cast<TabItemPtr>(Widget::baseCreateWidget(TabItem::getClassTypeName(), "Default", mItemTemplate->getCoord(), mItemTemplate->getAlign(), "", ""));
		_insertItem(_index, _name, sheet, _data);

		return sheet;
	}

	void Tab::setPosition(const IntPoint & _point)
	{
		Widget::setPosition(_point);
		updateBar();
	}

	void Tab::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
		updateBar();
	}

	void Tab::setCoord(const IntCoord & _coord)
	{
		Widget::setCoord(_coord);
		updateBar();
	}

	void Tab::updateBar()
	{
		// подстраховка
		if (mWidgetBar->getWidth() < 1) return;

		if ((mWidgetBar->getWidth() < mWidthBar) && (1 < mItemsInfo.size())) {
			if (false == mButtonShow) {
				mButtonShow = true;
				if (null != mButtonLeft) mButtonLeft->show();
				if (null != mButtonRight) mButtonRight->show();
				if (null != mButtonList) mButtonList->show();
				if (null != mButtonDecor) mButtonDecor->show();
				for (VectorWidgetPtr::iterator iter=mWidgetsPatch.begin(); iter!=mWidgetsPatch.end(); ++iter) (*iter)->show();
				mWidgetBar->setSize(mWidgetBar->getWidth() - mOffsetTab, mWidgetBar->getHeight());
			}
		}
		else {
			if (mButtonShow) {
				mButtonShow = false;
				if (null != mButtonLeft) mButtonLeft->hide();
				if (null != mButtonRight) mButtonRight->hide();
				if (null != mButtonList) mButtonList->hide();
				if (null != mButtonDecor) mButtonDecor->hide();
				for (VectorWidgetPtr::iterator iter=mWidgetsPatch.begin(); iter!=mWidgetsPatch.end(); ++iter) (*iter)->hide();
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
			button->show();

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
			mItemButton[count]->hide();
			count ++;
		}

		bool right = true;
		if (pos == mItemsInfo.size()) right = false;

		// корректируем виджет для пустоты
		if (width < mWidgetBar->getWidth()) {
			mEmptyBarWidget->show();
			mEmptyBarWidget->setCoord(width, 0, mWidgetBar->getWidth() - width, mWidgetBar->getHeight());
		}
		else {
			mEmptyBarWidget->hide();
		}

		// корректируем доступность стрелок
		if (mStartIndex == 0) {
			if (null != mButtonLeft) mButtonLeft->setEnabled(false);
		}
		else {
			if (null != mButtonLeft) mButtonLeft->setEnabled(true);
		}

		if (right) {
			if (null != mButtonRight) mButtonRight->setEnabled(true);
		}
		else {
			if (null != mButtonRight) mButtonRight->setEnabled(false);
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
			if (button->isShow()) {
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

	void Tab::setItemSelectedAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::setItemSelected");
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
		_widget->hide();
		_widget->setEnabled(true);
	}

	void Tab::_showItem(TabItemPtr _item, bool _show, bool _smooth)
	{
		if (false == _smooth) {
			ControllerManager::getInstance().removeItem(_item);
			_item->setAlpha(ALPHA_MAX);

			if (_show) _item->show();
			else _item->hide();

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

		IntSize size = mItemButton[0]->getTextSize();
		IntCoord coord = mItemButton[0]->getTextCoord();

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
				mItemsInfo[mIndexSelect].item->show();
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
			_sheet->hide();
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
