/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Tab.h"
#include "MyGUI_CastWidget.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	Tab::Tab(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mOffsetTab(0),
		mWidgetBar(null),
		mButtonLeft(null), mButtonRight(null), mButtonList(null),
		mEmptyBarWidget(null),
		mSheetTemplate(null),
		mButtonShow(false),
		mWidthBar(0),
		mStartIndex(0),
		mSelectSheet(ITEM_NONE),
		mButtonDefaultWidth(1),
		mButtonAutoWidth(true),
		mButtonSmoothShow(true)
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
			else if ((*iter)->getInternalString() == "Sheet") {
				mSheetTemplate = (*iter);
				mSheetTemplate->hide();
			}
		}
		MYGUI_ASSERT(null != mWidgetBar, "child is not find");
		MYGUI_ASSERT(null != mSheetTemplate, "child is not find");

		// создаем виджет, носитель скина пустоты бара
		mEmptyBarWidget = mWidgetBar->createWidget<Widget>(mEmptySkinName, IntCoord(), ALIGN_LEFT | ALIGN_TOP);

		/*mSheetsInfo.push_back(TabSheetInfo(100, "sheet 1", 0));
		mSheetsInfo.push_back(TabSheetInfo(130, "sheet 2", 0));
		mWidthBar = 230;
		mStartIndex = 1;
		mSelectSheet = 1;*/

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
				width += mSheetsInfo[mStartIndex].width;
			}
		}

		// проверяем и обновляем бар
		int width = 0;
		size_t count = 0;
		for (size_t pos=mStartIndex; pos<mSheetsInfo.size(); pos++) {
			if (width > mWidgetBar->getWidth()) break;

			// проверяем физическое наличие кнопки
			if (count >= mSheetButton.size()) _createSheetButton();

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
		size_t old = mSelectSheet;
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

		_showSheet(mSheetsInfo[mSelectSheet].sheet, true, mButtonSmoothShow);
		_showSheet(mSheetsInfo[old].sheet, false, mButtonSmoothShow);
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

	void Tab::setSheetButtonWidth(size_t _index, int _width)
	{
		MYGUI_ASSERT(_index < mSheetsInfo.size(), "index out of range");

		if (_width <= 0) {
			if (mButtonAutoWidth) _width = getButtonWidthByName(mSheetsInfo[_index].name);
			else _width = mButtonDefaultWidth;
		}

		mWidthBar += _width - mSheetsInfo[_index].width;
		mSheetsInfo[_index].width = _width;

		updateBar();
	}

	void Tab::setSheetName(size_t _index, const Ogre::DisplayString& _name, int _width)
	{
		MYGUI_ASSERT(_index < mSheetsInfo.size(), "index out of range");
		mSheetsInfo[_index].name = _name;

		if (_width <= 0) {
			if (mButtonAutoWidth) _width = getButtonWidthByName(_name);
			else _width = mButtonDefaultWidth;
		}

		mWidthBar += _width - mSheetsInfo[_index].width;
		mSheetsInfo[_index].width = _width;

		updateBar();
	}

	WidgetPtr Tab::insertSheet(size_t _index, const Ogre::DisplayString& _name, int _width)
	{
		if (_width <= 0) {
			if (mButtonAutoWidth) _width = getButtonWidthByName(_name);
			else _width = mButtonDefaultWidth;
		}

		mWidthBar += _width;
		WidgetPtr sheet = createWidget<Widget>("Empty", mSheetTemplate->getCoord(), mSheetTemplate->getAlign());

		// положение и видимость вкладок
		if (0 == mSheetsInfo.size()) mSelectSheet = 0;
		else {
			if (_index <= mSelectSheet) mSelectSheet ++;
			sheet->hide();
		}

		if (_index >= mSheetsInfo.size()) mSheetsInfo.push_back(TabSheetInfo(_width, _name, sheet));
		else mSheetsInfo.insert(mSheetsInfo.begin() + _index, TabSheetInfo(_width, _name, sheet));

		updateBar();

		return sheet;
	}

	void Tab::removeSheetIndex(size_t _index)
	{
		MYGUI_ASSERT(_index < mSheetsInfo.size(), "index out of range");

		// удаляем страницу
		TabSheetInfo & info = mSheetsInfo[_index];
		_removeFromAlphaController(info.sheet);
		WidgetManager::getInstance().destroyWidget(info.sheet);
		mWidthBar -= info.width;
		mSheetsInfo.erase(mSheetsInfo.begin() + _index);

		if (0 == mSheetsInfo.size()) mSelectSheet = ITEM_NONE;
		else {
			if (_index < mSelectSheet) mSelectSheet --;
			else if (_index == mSelectSheet) {
				if (mSelectSheet == mSheetsInfo.size()) mSelectSheet --;
				mSheetsInfo[mSelectSheet].sheet->show();
			}
		}

		updateBar();
	}

	void Tab::removeSheet(const Ogre::DisplayString& _name)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].name == _name) {
				removeSheetIndex(pos);
				return;
			}
		}
		MYGUI_EXCEPT("sheet '" << _name << "' is not find");
	}

	void Tab::removeSheet(WidgetPtr _sheet)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].sheet == _sheet) {
				removeSheetIndex(pos);
				return;
			}
		}
		MYGUI_EXCEPT("sheet (" << _sheet << ") is not find");
	}

	void Tab::selectSheetIndex(size_t _index, bool _smoot)
	{
		MYGUI_ASSERT(_index < mSheetsInfo.size(), "index out of range");
		if (mSelectSheet == _index) return;
		size_t old = mSelectSheet;
		mSelectSheet = _index;
		updateBar();

		_showSheet(mSheetsInfo[mSelectSheet].sheet, true, _smoot);
		_showSheet(mSheetsInfo[old].sheet, false, _smoot);
	}

	void Tab::selectSheet(const Ogre::DisplayString& _name, bool _smooth)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].name == _name) {
				selectSheetIndex(pos, _smooth);
				return;
			}
		}
		MYGUI_EXCEPT("sheet '" << _name << "' is not find");
	}

	void Tab::selectSheet(WidgetPtr _sheet, bool _smooth)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].sheet == _sheet) {
				selectSheetIndex(pos, _smooth);
				return;
			}
		}
		MYGUI_EXCEPT("sheet (" << _sheet << ") is not find");
	}

	void Tab::_showSheet(WidgetPtr _sheet, bool _show, bool _smooth)
	{
		if (false == _smooth) {
			_removeFromAlphaController(_sheet);
			_sheet->setAlpha(ALPHA_MAX);

			if (_show) _sheet->show();
			else _sheet->hide();

			return;
		}

		if (_show) _addToAlphaController(_sheet, ALPHA_MAX);
		else _addToAlphaController(_sheet, ALPHA_MIN, true, false);
	}

	void Tab::_addToAlphaController(WidgetPtr _widget, float _alpha, bool _hide, bool _enabled, bool _destroy)
	{
		// подготовка виджета
		_widget->setEnabled(_enabled, true);

		if ((ALPHA_MIN != _alpha) && (false == _widget->isShow())) {
			_widget->setAlpha(ALPHA_MIN);
			_widget->show();
		}

		size_t size = mVectorAlphaDataInfo.size();
		for (size_t pos=0; pos<size; ++pos) {
			if (mVectorAlphaDataInfo[pos].widget == _widget) {
				mVectorAlphaDataInfo[pos].alpha = _alpha;
				mVectorAlphaDataInfo[pos].destroy = _destroy;
				mVectorAlphaDataInfo[pos].hide = _hide;
				return;
			}
		}
		mVectorAlphaDataInfo.push_back(AlphaDataInfo(_widget, _alpha, _hide, _destroy));

		if (0 == size) Gui::getInstance().addFrameListener(this);
	}

	void Tab::_removeFromAlphaController(WidgetPtr _widget)
	{
		size_t size = mVectorAlphaDataInfo.size();
		for (size_t pos=0; pos<size; ++pos) {
			if (mVectorAlphaDataInfo[pos].widget == _widget) {
				mVectorAlphaDataInfo[pos] = mVectorAlphaDataInfo[size-1];
				mVectorAlphaDataInfo.pop_back();
				break;
			}
		}

		if (1 == size) Gui::getInstance().removeFrameListener(this);
	}

	int Tab::getButtonWidthByName(const Ogre::DisplayString& _text)
	{
		if (0 == mSheetButton.size()) _createSheetButton();

		IntSize size = mSheetButton[0]->getTextSize(_text);
		IntCoord coord = mSheetButton[0]->getTextCoord();

		return size.width + mSheetButton[0]->getWidth() - coord.width;
	}

	void Tab::_frameEntered(float _frame)
	{
		const float coef = 4.0f;

		for (size_t pos=0; pos<mVectorAlphaDataInfo.size(); pos++) {

			AlphaDataInfo & info = mVectorAlphaDataInfo[pos];
			float alpha = info.widget->getAlpha();

			// проверяем нужно ли к чему еще стремиться
			if (info.alpha > alpha) {
				alpha += _frame * coef;
				if (info.alpha > alpha) {
					info.widget->setAlpha(alpha);
					continue;
				}
			}
			else if (info.alpha < alpha) {
				alpha -= _frame * coef;
				if (info.alpha < alpha) {
					info.widget->setAlpha(alpha);
					continue;
				}
			}

			// если мы тут значит событие свершилось
			if (info.destroy) WidgetManager::getInstance().destroyWidget(info.widget);
			else if (info.hide) info.widget->hide();

			// удаляем себя из списка
			mVectorAlphaDataInfo[pos] = mVectorAlphaDataInfo[mVectorAlphaDataInfo.size()-1];
			mVectorAlphaDataInfo.pop_back();
			pos --;

		}
	}

} // namespace MyGUI
