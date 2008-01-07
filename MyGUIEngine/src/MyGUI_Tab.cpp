/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Tab.h"
#include "MyGUI_CastWidget.h"

namespace MyGUI
{

	Tab::Tab(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mOffsetTab(0),
		mWidgetBar(null),
		mButtonLeft(null), mButtonRight(null), mButtonList(null),
		mEmptyBarWidget(null),
		mButtonShow(false),
		mWidthBar(0),
		mStartIndex(0),
		mSelectSheet(ITEM_NONE)
	{

		// парсим свойства
		const MapString & param = _info->getParams();
		if (false == param.empty()) {
			MapString::const_iterator iter = param.find("OffsetBar");
			if (iter != param.end()) mOffsetTab = util::parseInt(iter->second);

			iter = param.find("ButtonSkin");
			if (iter != param.end()) mButtonSkinName = iter->second;
			iter = param.find("EmptyBarSkin");
			if (iter != param.end()) mEmptySkinName = iter->second;
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->getInternalString() == "Bar") {
				mWidgetBar = (*iter);
				//mWidgetScroll->eventScrollChangePosition = newDelegate(this, &List::notifyScrollChangePosition);
				//mWidgetScroll->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
			else if ((*iter)->getInternalString() == "Left") {
				mButtonLeft = castWidget<Button>(*iter);
				mButtonLeft->hide();
				mButtonLeft->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if ((*iter)->getInternalString() == "Right") {
				mButtonRight = castWidget<Button>(*iter);
				mButtonRight->hide();
				mButtonRight->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if ((*iter)->getInternalString() == "List") {
				mButtonList = castWidget<Button>(*iter);
				mButtonList->hide();
				mButtonList->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if ((*iter)->getInternalString() == "ShowPatch") {
				mWidgetsPatch.push_back((*iter));
				(*iter)->hide();
			}
		}
		MYGUI_ASSERT(null != mWidgetBar, "child is not find");

		// создаем виджет, носитель скина пустоты бара
		mEmptyBarWidget = mWidgetBar->createWidget<Widget>(mEmptySkinName, IntCoord(), ALIGN_LEFT | ALIGN_TOP);

		mSheetsInfo.push_back(TabSheetInfo(100, "sheet 1", 0));
		mSheetsInfo.push_back(TabSheetInfo(130, "sheet 2", 0));
		mWidthBar = 230;
		mStartIndex = 1;
		mSelectSheet = 1;

		updateBar();
	}

	void Tab::setPosition(const IntCoord& _coord)
	{
		Widget::setPosition(_coord);
		updateBar();
	}

	void Tab::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
		updateBar();
	}

	void Tab::updateBar()
	{
		// подстраховка
		if (mWidgetBar->getWidth() < 1) return;

		if ((mWidgetBar->getWidth() < mWidthBar) && (1 < mSheetsInfo.size())) {
			if (false == mButtonShow) {
				mButtonShow = true;
				if (null != mButtonLeft) mButtonLeft->show();
				if (null != mButtonRight) mButtonRight->show();
				if (null != mButtonList) mButtonList->show();
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
				for (VectorWidgetPtr::iterator iter=mWidgetsPatch.begin(); iter!=mWidgetsPatch.end(); ++iter) (*iter)->hide();
				mWidgetBar->setSize(mWidgetBar->getWidth() + mOffsetTab, mWidgetBar->getHeight());
			}
		}

		// проверяем правильность стартового индекса
		if (mStartIndex > 0) {
			// считаем длинну видимых кнопок
			int width = 0;
			for (size_t pos=mStartIndex; pos<mSheetsInfo.size(); pos++) width += mSheetsInfo[pos].width;

			// уменьшаем индекс до тех пор пока кнопка до индекста полностью не влезет в бар
			while ((mStartIndex > 0) && ((width + mSheetsInfo[mStartIndex-1].width) <= mWidgetBar->getWidth())) {
				mStartIndex--;
			}
		}

		// проверяем и обновляем бар
		int width = 0;
		size_t count = 0;
		for (size_t pos=mStartIndex; pos<mSheetsInfo.size(); pos++) {
			if (width > mWidgetBar->getWidth()) break;

			// проверяем физическое наличие кнопки
			if (count >= mSheetButton.size()) {
				ButtonPtr button = mWidgetBar->createWidget<Button>(mButtonSkinName, IntCoord(), ALIGN_LEFT | ALIGN_TOP);
				button->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedBarButtonEvent);
				button->setInternalData((int)count); // порядковый номер
				mSheetButton.push_back(button);
			}

			// если кнопка не соответствует, то изменяем ее
			TabSheetInfo & info = mSheetsInfo[pos]; 
			ButtonPtr button = castWidget<Button>(mSheetButton[count]);
			button->show();

			// корректируем нажатость кнопки
			if (pos == mSelectSheet) {
				if (false == button->getButtonPressed()) button->setButtonPressed(true);
			}
			else if (button->getButtonPressed()) button->setButtonPressed(false);

			if (button->getCaption() != info.name)
				button->setCaption(info.name);
			// положение кнопки
			IntCoord coord(width, 0, info.width, mWidgetBar->getHeight());
			if (coord != button->getCoord())
				button->setPosition(coord);

			width += info.width;
			count ++;
		}

		// скрываем кнопки что были созданны, но не видны
		while (count < mSheetButton.size()) {
			mSheetButton[count]->hide();
			count ++;
		}

		// корректируем виджет для пустоты
		if (width < mWidgetBar->getWidth()) {
			mEmptyBarWidget->show();
			mEmptyBarWidget->setPosition(width, 0, mWidgetBar->getWidth() - width, mWidgetBar->getHeight());
		}
		else {
			mEmptyBarWidget->hide();
		}

	}

	void Tab::notifyPressedButtonEvent(MyGUI::WidgetPtr _sender, bool _double)
	{
		if (_double) return;

		if (_sender == mButtonLeft) {
			if (mStartIndex > 0) {
				mStartIndex --;
				updateBar();
			}
		}
		else if (_sender == mButtonRight) {
			if ((mStartIndex+1) < mSheetsInfo.size()) {
				mStartIndex ++;
				// в updateBar() будет подкорректированно если что
				updateBar();
			}
		}
		else if (_sender == mButtonList) {
		}
	}

	void Tab::notifyPressedBarButtonEvent(MyGUI::WidgetPtr _sender, bool _double)
	{
		if (_double) return;

		size_t select = (size_t)_sender->getInternalData() + mStartIndex;
		// щелкнули по той же кнопке
		if (select == mSelectSheet) {
			// стараемся показать выделенную кнопку
			showBarSelectButton();
			return;
		}

		mSelectSheet = select;

		size_t count = 0;
		for (size_t pos=mStartIndex; pos<mSheetsInfo.size(); pos++) {
			ButtonPtr button = castWidget<Button>(mSheetButton[count]);
			if (button->isShow()) {
				// корректируем нажатость кнопки
				if (pos == mSelectSheet) {
					if (false == button->getButtonPressed()) button->setButtonPressed(true);
				}
				else if (button->getButtonPressed()) button->setButtonPressed(false);
			}
			count ++;
		}

		// стараемся показать выделенную кнопку
		showBarSelectButton();
	}

	void Tab::showBarButton(size_t _index)
	{
		if (_index >= mSheetsInfo.size()) {
			MYGUI_LOG(Warning, "index in sheets out of range");
			return;
		}

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
				width += mSheetsInfo[pos].width;
			}

			// уменьшем старт индекс пока не появиться нужная
			bool change = false;
			while ((mStartIndex < _index) && (width > mWidgetBar->getWidth())) {
				width -= mSheetsInfo[mStartIndex].width;
				mStartIndex ++;
				change = true;
			}
			if (change) updateBar();

		}
	}

	void Tab::showBarButton(const Ogre::DisplayString& _name)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].name == _name) {
				showBarButton(pos);
				return;
			}
		}
		MYGUI_LOG(Warning, "sheet '" << _name << "' is not find");
	}

} // namespace MyGUI
