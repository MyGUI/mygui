/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Tab.h"
#include "MyGUI_CastWidget.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_Button.h"
#include "MyGUI_Sheet.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	Ogre::String Tab::WidgetTypeName = "Tab";

	const float TAB_SPEED_FADE_COEF = 5.0f;

	Tab::Tab(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
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
		mSmoothShow(true),
		mShutDown(false)
	{

		// ������ ��������
		const MapString & param = _info->getParams();
		if (false == param.empty()) {
			MapString::const_iterator iter = param.find("OffsetBar");
			if (iter != param.end()) mOffsetTab = utility::parseInt(iter->second);

			iter = param.find("ButtonSkin");
			if (iter != param.end()) mButtonSkinName = iter->second;
			iter = param.find("EmptyBarSkin");
			if (iter != param.end()) mEmptySkinName = iter->second;
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Bar") {
				mWidgetBar = (*iter);
			}
			else if ((*iter)->_getInternalString() == "Left") {
				mButtonLeft = castWidget<Button>(*iter);
				mButtonLeft->hide();
				mButtonLeft->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if ((*iter)->_getInternalString() == "Right") {
				mButtonRight = castWidget<Button>(*iter);
				mButtonRight->hide();
				mButtonRight->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if ((*iter)->_getInternalString() == "List") {
				mButtonList = castWidget<Button>(*iter);
				mButtonList->hide();
				mButtonList->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedButtonEvent);
			}
			else if ((*iter)->_getInternalString() == "ShowPatch") {
				mWidgetsPatch.push_back((*iter));
				(*iter)->hide();
			}
			else if ((*iter)->_getInternalString() == "Sheet") {
				mSheetTemplate = (*iter);
				mSheetTemplate->hide();
			}
		}
		MYGUI_ASSERT(null != mWidgetBar, "Child Widget Bar not found in skin (Tab must have Bar)");
		MYGUI_ASSERT(null != mSheetTemplate, "Child Widget Sheet not found in skin (Tab must have Sheet)");

		// ������� ������, �������� ����� ������� ����
		mEmptyBarWidget = mWidgetBar->createWidget<Widget>(mEmptySkinName, IntCoord(), ALIGN_LEFT | ALIGN_TOP);

		updateBar();
	}

	Tab::~Tab()
	{
		mShutDown = true;
		// ������ ������� ������, ������� ���� ��������
		// � ������� ��� �������� ���� �� ������ � ������
		//mSheetsInfo.clear();
	}

	// �������������� ��� ������� ������������ �������
	WidgetPtr Tab::_createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		if (Sheet::_getType() == _type) {
			SheetPtr sheet = static_cast<SheetPtr>(Widget::_createWidget(_type, "Default", mSheetTemplate->getCoord(), mSheetTemplate->getAlign(), _layer, _name));
			//sheet->mOwner = this;

			// ��������� ���� � �������
			int width = (mButtonAutoWidth ? getButtonWidthByName("") : mButtonDefaultWidth);
			mWidthBar += width;

			// ��������� � ��������� �������
			if (0 == mSheetsInfo.size()) mSelectSheet = 0;
			else sheet->hide();

			mSheetsInfo.push_back(TabSheetInfo(width, "", sheet));
			updateBar();

			return sheet;
		}
		return Widget::_createWidget(_type, _skin, _coord, _align, _layer, _name);
	}

	SheetPtr Tab::addSheet(const Ogre::DisplayString& _name, int _width)
	{
		SheetPtr sheet = static_cast<SheetPtr>(createWidgetT(Sheet::_getType(), "", IntCoord(), ALIGN_DEFAULT));
		setSheetNameIndex(mSheetsInfo.size()-1, _name, _width);
		return sheet;
	}

	SheetPtr Tab::insertSheet(size_t _index, const Ogre::DisplayString& _name, int _width)
	{
		SheetPtr sheet = addSheet(_name, _width);
		size_t size = mSheetsInfo.size();
		if ((_index >= mSheetsInfo.size()) || (1 == size) ) return sheet;

		// ������ ������ � ������������ ���������� �������
		TabSheetInfo tmp = mSheetsInfo[size-1];
		mSheetsInfo[size-1] = mSheetsInfo[_index];
		mSheetsInfo[_index] = tmp;
	
		if (_index <= mSelectSheet) mSelectSheet ++;

		updateBar();

		return sheet;
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
		// ������������
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

		// ��������� ������������ ���������� �������
		if (mStartIndex > 0) {
			// ������� ������ ������� ������
			int width = 0;
			for (size_t pos=mStartIndex; pos<mSheetsInfo.size(); pos++) width += mSheetsInfo[pos].width;

			// ��������� ������ �� ��� ��� ���� ������ �� �������� ��������� �� ������ � ���
			while ((mStartIndex > 0) && ((width + mSheetsInfo[mStartIndex-1].width) <= mWidgetBar->getWidth())) {
				mStartIndex--;
				width += mSheetsInfo[mStartIndex].width;
			}
		}

		// ��������� � ��������� ���
		int width = 0;
		size_t count = 0;
		for (size_t pos=mStartIndex; pos<mSheetsInfo.size(); pos++) {
			if (width > mWidgetBar->getWidth()) break;

			// ��������� ���������� ������� ������
			if (count >= mSheetButton.size()) _createSheetButton();

			// ���� ������ �� �������������, �� �������� ��
			TabSheetInfo & info = mSheetsInfo[pos]; 
			ButtonPtr button = castWidget<Button>(mSheetButton[count]);
			button->show();

			// ������������ ��������� ������
			if (pos == mSelectSheet) {
				if (false == button->getButtonPressed()) button->setButtonPressed(true);
			}
			else if (button->getButtonPressed()) button->setButtonPressed(false);

			if (button->getCaption() != info.name)
				button->setCaption(info.name);
			// ��������� ������
			IntCoord coord(width, 0, info.width, mWidgetBar->getHeight());
			if (coord != button->getCoord())
				button->setPosition(coord);

			width += info.width;
			count ++;
		}

		// �������� ������ ��� ���� ��������, �� �� �����
		while (count < mSheetButton.size()) {
			mSheetButton[count]->hide();
			count ++;
		}

		// ������������ ������ ��� �������
		if (width < mWidgetBar->getWidth()) {
			mEmptyBarWidget->show();
			mEmptyBarWidget->setPosition(width, 0, mWidgetBar->getWidth() - width, mWidgetBar->getHeight());
		}
		else {
			mEmptyBarWidget->hide();
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
			if ((mStartIndex+1) < mSheetsInfo.size()) {
				mStartIndex ++;
				// � updateBar() ����� ������������������ ���� ���
				updateBar();
			}
		}
		else if (_sender == mButtonList) {
		}
	}

	void Tab::notifyPressedBarButtonEvent(MyGUI::WidgetPtr _sender)
	{
		size_t select = (size_t)_sender->_getInternalData() + mStartIndex;
		// �������� �� ��� �� ������
		if (select == mSelectSheet) {
			// ��������� �������� ���������� ������
			showBarSelectButton();
			return;
		}
		size_t old = mSelectSheet;
		mSelectSheet = select;

		size_t count = 0;
		for (size_t pos=mStartIndex; pos<mSheetsInfo.size(); pos++) {
			ButtonPtr button = castWidget<Button>(mSheetButton[count]);
			if (button->isShow()) {
				// ������������ ��������� ������
				if (pos == mSelectSheet) {
					if (false == button->getButtonPressed()) button->setButtonPressed(true);
				}
				else if (button->getButtonPressed()) button->setButtonPressed(false);
			}
			count ++;
		}

		// ��������� �������� ���������� ������
		showBarSelectButton();

		// ��������� �������� ��� �������
		_forcePeek(mSheetsInfo[mSelectSheet].sheet);

		_showSheet(mSheetsInfo[mSelectSheet].sheet, true, mSmoothShow);
		_showSheet(mSheetsInfo[old].sheet, false, mSmoothShow);
	}

	void Tab::showBarButton(size_t _index)
	{
		if (_index >= mSheetsInfo.size()) {
			MYGUI_LOG(Warning, "index in sheets out of range");
			return;
		}

		// ������������
		if (mWidgetBar->getWidth() < 1) return;

		if (_index == mStartIndex) return;
		else if (_index < mStartIndex) {
			mStartIndex = _index;
			updateBar();
		}
		else {
			// ������ ���� �� ����� ������� �� ������ ������������
			int width = 0;
			for (size_t pos=mStartIndex; pos<=_index; pos++) {
				width += mSheetsInfo[pos].width;
			}

			// �������� ����� ������ ���� �� ��������� ������
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
		MYGUI_LOG(Warning, "sheet '" << _name << "' not found");
	}

	void Tab::setSheetButtonWidthIndex(size_t _index, int _width)
	{
		MYGUI_ASSERT(_index < mSheetsInfo.size(), "setSheetButtonWidthIndex: index '" << _index << "' out of range");

		if (_width <= 0) {
			if (mButtonAutoWidth) _width = getButtonWidthByName(mSheetsInfo[_index].name);
			else _width = mButtonDefaultWidth;
		}

		mWidthBar += _width - mSheetsInfo[_index].width;
		mSheetsInfo[_index].width = _width;

		updateBar();
	}

	void Tab::setSheetNameIndex(size_t _index, const Ogre::DisplayString& _name, int _width)
	{
		MYGUI_ASSERT(_index < mSheetsInfo.size(), "setSheetNameIndex: index '" << _index << "' out of range");
		mSheetsInfo[_index].name = _name;

		if (_width <= 0) {
			if (mButtonAutoWidth) _width = getButtonWidthByName(_name);
			else _width = mButtonDefaultWidth;
		}

		mWidthBar += _width - mSheetsInfo[_index].width;
		mSheetsInfo[_index].width = _width;

		updateBar();
	}

	void Tab::setSheetName(SheetPtr _sheet, const Ogre::DisplayString& _name, int _width)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].sheet == _sheet) {
				setSheetNameIndex(pos, _name, _width);
				return;
			}
		}
		MYGUI_EXCEPT("sheet (" << _sheet << ") not found");
	}

	void Tab::setSheetButtonWidth(SheetPtr _sheet, int _width)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].sheet == _sheet) {
				setSheetButtonWidthIndex(pos, _width);
				return;
			}
		}
		MYGUI_EXCEPT("sheet (" << _sheet << ") not found");
	}

	void Tab::removeSheetIndex(size_t _index)
	{
		MYGUI_ASSERT(_index < mSheetsInfo.size(), "removeSheetIndex: index '" << _index << "' out of range");

		this->_destroyChildWidget(mSheetsInfo[_index].sheet);
		// ������� ��������
		/*TabSheetInfo & info = mSheetsInfo[_index];
		// ��� �������� ��� ����������
		//ControllerManager::getInstance().removeItem(info.sheet);
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

		updateBar();*/
	}

	void Tab::removeSheet(const Ogre::DisplayString& _name)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].name == _name) {
				removeSheetIndex(pos);
				return;
			}
		}
		MYGUI_EXCEPT("sheet '" << _name << "' not found");
	}

	void Tab::removeSheet(SheetPtr _sheet)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].sheet == _sheet) {
				removeSheetIndex(pos);
				return;
			}
		}
		MYGUI_EXCEPT("sheet (" << _sheet << ") not found");
	}

	void Tab::selectSheetIndex(size_t _index, bool _smoot)
	{
		MYGUI_ASSERT(_index < mSheetsInfo.size(), "selectSheetIndex: index '" << _index << "' out of range");
		if (mSelectSheet == _index) return;
		size_t old = mSelectSheet;
		mSelectSheet = _index;
		updateBar();

		// ��������� �������� ��� �������
		if (_smoot) _forcePeek(mSheetsInfo[mSelectSheet].sheet);

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
		MYGUI_EXCEPT("sheet '" << _name << "' not found");
	}

	void Tab::selectSheet(SheetPtr _sheet, bool _smooth)
	{
		for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
			if (mSheetsInfo[pos].sheet == _sheet) {
				selectSheetIndex(pos, _smooth);
				return;
			}
		}
		MYGUI_EXCEPT("sheet (" << _sheet << ") not found");
	}

	void Tab::_showSheet(SheetPtr _sheet, bool _show, bool _smooth)
	{
		if (false == _smooth) {
			ControllerManager::getInstance().removeItem(_sheet);
			_sheet->setAlpha(ALPHA_MAX);

			if (_show) _sheet->show();
			else _sheet->hide();

			return;
		}

		if (_show) {
			ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MAX, TAB_SPEED_FADE_COEF, true);
			ControllerManager::getInstance().addItem(_sheet, controller);
		}
		else {
			ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, TAB_SPEED_FADE_COEF, false);
			controller->eventPostAction = newDelegate(action::actionWidgetHide);
			ControllerManager::getInstance().addItem(_sheet, controller);
		}
	}

	void Tab::_createSheetButton()
	{
		ButtonPtr button = mWidgetBar->createWidget<Button>(mButtonSkinName, IntCoord(), ALIGN_LEFT | ALIGN_TOP);
		button->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedBarButtonEvent);
		button->_setInternalData((int)mSheetButton.size()); // ���������� �����
		mSheetButton.push_back(button);
	}

	int Tab::getButtonWidthByName(const Ogre::DisplayString& _text)
	{
		if (0 == mSheetButton.size()) _createSheetButton();

		Ogre::DisplayString save = mSheetButton[0]->getCaption();
		mSheetButton[0]->setCaption(_text);

		IntSize size = mSheetButton[0]->getTextSize();
		IntCoord coord = mSheetButton[0]->getTextCoord();

		mSheetButton[0]->setCaption(save);

		return size.width + mSheetButton[0]->getWidth() - coord.width;
	}

	void Tab::_notifyDeleteSheet(SheetPtr _sheet)
	{
		// ����� ������� �������
		if (mShutDown) return;

		for (VectorTabSheetInfo::iterator iter=mSheetsInfo.begin(); iter!=mSheetsInfo.end(); ++iter) {
			if ((*iter).sheet == _sheet) {

				//TabSheetInfo & info = mSheetsInfo[_index];
				// ��� �������� ��� ����������
				//ControllerManager::getInstance().removeItem(info.sheet);
				//WidgetManager::getInstance().destroyWidget(info.sheet);
				size_t index = iter - mSheetsInfo.begin();

				mWidthBar -= (*iter).width;
				mSheetsInfo.erase(iter);

				if (0 == mSheetsInfo.size()) mSelectSheet = ITEM_NONE;
				else {
					if (index < mSelectSheet) mSelectSheet --;
					else if (index == mSelectSheet) {
						if (mSelectSheet == mSheetsInfo.size()) mSelectSheet --;
						mSheetsInfo[mSelectSheet].sheet->show();
						mSheetsInfo[mSelectSheet].sheet->setAlpha(ALPHA_MAX);
					}
				}

				updateBar();

				//removeSheetIndex(pos);
				return;
			}
		}
		MYGUI_EXCEPT("sheet (" << _sheet << ") not found");
	}

} // namespace MyGUI
