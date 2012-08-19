/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "Precompiled.h"
#include "SeparatePanel.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SeparatePanel)

	SeparatePanel::SeparatePanel() :
		mFirstPanel(),
		mSecondPanel(),
		mSeparatorH(),
		mSeparatorV(),
		mMinSize(),
		mPanleAlign(MyGUI::Align::Left),
		mFirstPanelSize()
	{
	}

	SeparatePanel::~SeparatePanel()
	{
	}

	void SeparatePanel::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, "SeparatePanel.layout");

		assignWidget(mFirstPanel, "FirstPanel");
		assignWidget(mSecondPanel, "SecondPanel");
		assignWidget(mSeparatorH, "SeparatorH");
		assignWidget(mSeparatorV, "SeparatorV");

		mMinSize = MyGUI::utility::parseValue<int>(mMainWidget->getUserString("MinSize"));

		mMainWidget->eventChangeCoord += MyGUI::newDelegate(this, &SeparatePanel::notifyChangeCoord);
		mSeparatorH->eventMouseButtonPressed += MyGUI::newDelegate(this, &SeparatePanel::notifyMouseButtonPressed);
		mSeparatorV->eventMouseButtonPressed += MyGUI::newDelegate(this, &SeparatePanel::notifyMouseButtonPressed);
		mSeparatorH->eventMouseDrag += MyGUI::newDelegate(this, &SeparatePanel::notifyMouseDrag);
		mSeparatorV->eventMouseDrag += MyGUI::newDelegate(this, &SeparatePanel::notifyMouseDrag);

		setFirstCoords();

		setPanelAlign(MyGUI::Align::Bottom);
	}

	void SeparatePanel::notifyChangeCoord(MyGUI::Widget* _sender)
	{
		invalidateSize(mFirstPanel->getCoord(), mSeparatorH->getCoord(), mSeparatorV->getCoord(), mSecondPanel->getCoord());
	}

	void SeparatePanel::invalidateSize(const MyGUI::IntCoord& _firstPanel, const MyGUI::IntCoord& _separatorH, const MyGUI::IntCoord& _separatorV, const MyGUI::IntCoord& _secondPanel)
	{
		if (mMainWidget->getWidth() <= 0 ||
			mMainWidget->getHeight() <= 0)
			return;

		MyGUI::IntCoord firstPanel = _firstPanel;
		MyGUI::IntCoord separatorH = _separatorH;
		MyGUI::IntCoord separatorV = _separatorV;
		MyGUI::IntCoord secondPanel = _secondPanel;

		if (mPanleAlign.isLeft())
		{
			updateSize(firstPanel.width, secondPanel.width, separatorH.width, mMainWidget->getWidth(), mFirstPanelSize.width);
			firstPanel.top = separatorH.top = secondPanel.top = 0;
			firstPanel.height = separatorH.height = secondPanel.height = mMainWidget->getHeight();
			firstPanel.left = 0;
			separatorH.left = firstPanel.right();
			secondPanel.left = separatorH.right();
		}
		else if (mPanleAlign.isRight())
		{
			updateSize(firstPanel.width, secondPanel.width, separatorH.width, mMainWidget->getWidth(), mFirstPanelSize.width);
			firstPanel.top = separatorH.top = secondPanel.top = 0;
			firstPanel.height = separatorH.height = secondPanel.height = mMainWidget->getHeight();
			secondPanel.left = 0;
			separatorH.left = secondPanel.right();
			firstPanel.left = separatorH.right();
		}
		else if (mPanleAlign.isTop())
		{
			updateSize(firstPanel.height, secondPanel.height, separatorV.height, mMainWidget->getHeight(), mFirstPanelSize.height);
			firstPanel.left = separatorV.left = secondPanel.left = 0;
			firstPanel.width = separatorV.width = secondPanel.width = mMainWidget->getWidth();
			firstPanel.top = 0;
			separatorV.top = firstPanel.bottom();
			secondPanel.top = separatorV.bottom();
		}
		else if (mPanleAlign.isBottom())
		{
			updateSize(firstPanel.height, secondPanel.height, separatorV.height, mMainWidget->getHeight(), mFirstPanelSize.height);
			firstPanel.left = separatorV.left = secondPanel.left = 0;
			firstPanel.width = separatorV.width = secondPanel.width = mMainWidget->getWidth();
			secondPanel.top = 0;
			separatorV.top = secondPanel.bottom();
			firstPanel.top = separatorV.bottom();
		}

		if (firstPanel != mFirstPanel->getCoord())
			mFirstPanel->setCoord(firstPanel);

		if (separatorH != mSeparatorH->getCoord())
			mSeparatorH->setCoord(separatorH);

		if (separatorV != mSeparatorV->getCoord())
			mSeparatorV->setCoord(separatorV);

		if (secondPanel != mSecondPanel->getCoord())
			mSecondPanel->setCoord(secondPanel);
	}

	void SeparatePanel::setPanelAlign(MyGUI::Align _value)
	{
		mPanleAlign = _value;

		if (mPanleAlign.isLeft() || mPanleAlign.isRight())
		{
			mSeparatorH->setVisible(true);
			mSeparatorV->setVisible(false);
		}
		else if (mPanleAlign.isTop() || mPanleAlign.isBottom())
		{
			mSeparatorH->setVisible(false);
			mSeparatorV->setVisible(true);
		}

		invalidateSize(mFirstPanel->getCoord(), mSeparatorH->getCoord(), mSeparatorV->getCoord(), mSecondPanel->getCoord());
	}

	MyGUI::Align SeparatePanel::getPanelAlign() const
	{
		return mPanleAlign;
	}

	void SeparatePanel::updateSize(int& _firstWidth, int& _secondWidth, int _separatorWidth, int _mainWidth, int _defaultSize)
	{
		/*if (_firstWidth < _defaultSize)
			_firstWidth = _defaultSize;*/

		if (_firstWidth < mMinSize)
			_firstWidth = mMinSize;

		if (_secondWidth < mMinSize)
			_secondWidth = mMinSize;

		if (_firstWidth + _secondWidth + _separatorWidth > _mainWidth)
		{
			_secondWidth = _mainWidth - _firstWidth - _separatorWidth;

			if (_secondWidth < mMinSize)
				_secondWidth = mMinSize;

			if (_firstWidth + _secondWidth + _separatorWidth > _mainWidth)
			{
				_firstWidth = _mainWidth - _secondWidth - _separatorWidth;

				if (_firstWidth < mMinSize)
					_firstWidth = mMinSize;

				if (_firstWidth + _secondWidth + _separatorWidth > _mainWidth)
				{
					_secondWidth = _mainWidth - _firstWidth - _separatorWidth;

					if (_secondWidth < 0)
						_secondWidth = 0;

					if (_firstWidth + _secondWidth + _separatorWidth > _mainWidth)
					{
						_firstWidth = _mainWidth - _secondWidth - _separatorWidth;

						if (_firstWidth < 0)
							_firstWidth = 0;
					}
				}
			}
		}

		if (_firstWidth + _secondWidth + _separatorWidth < _mainWidth)
			_secondWidth = _mainWidth - _firstWidth - _separatorWidth;
	}

	void SeparatePanel::setFirstCoords()
	{
		MyGUI::IntSize size = MyGUI::IntSize(
			(mMainWidget->getWidth() - mSeparatorH->getWidth()) / 2,
			(mMainWidget->getHeight() - mSeparatorH->getHeight()) / 2);

		mFirstPanel->setSize(size);
		mSecondPanel->setSize(size);
	}

	void SeparatePanel::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id != MyGUI::MouseButton::Left)
			return;

		mMousePressedOffset = MyGUI::IntPoint(_left, _top) - _sender->getAbsolutePosition();
	}

	void SeparatePanel::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id != MyGUI::MouseButton::Left)
			return;

		SeparatePanel::moveSeparator(MyGUI::IntPoint(_left, _top));
	}

	void SeparatePanel::moveSeparator(const MyGUI::IntPoint& _mousePosition)
	{
		MyGUI::IntPoint offset = _mousePosition - mMousePressedOffset - mMainWidget->getAbsolutePosition();

		MyGUI::IntCoord firstPanel = mFirstPanel->getCoord();
		MyGUI::IntCoord separatorH = mSeparatorH->getCoord();
		MyGUI::IntCoord separatorV = mSeparatorV->getCoord();
		MyGUI::IntCoord secondPanel = mSecondPanel->getCoord();

		if (mPanleAlign.isLeft())
		{
			firstPanel.width = offset.left - firstPanel.left;
			separatorH.left = offset.left;
			secondPanel.width = secondPanel.right() - (offset.left + separatorH.width);
			secondPanel.left = offset.left + separatorH.width;
		}
		else if (mPanleAlign.isRight())
		{
			secondPanel.width = offset.left - secondPanel.left;
			separatorH.left = offset.left;
			firstPanel.width = firstPanel.right() - (offset.left + separatorH.width);
			firstPanel.left = offset.left + separatorH.width;
		}
		else if (mPanleAlign.isTop())
		{
			firstPanel.height = offset.top - firstPanel.top;
			separatorV.top = offset.top;
			secondPanel.height = secondPanel.bottom() - (offset.top + separatorV.height);
			secondPanel.top = offset.top + separatorV.height;
		}
		else if (mPanleAlign.isBottom())
		{
			secondPanel.height = offset.top - secondPanel.top;
			separatorV.top = offset.top;
			firstPanel.height = firstPanel.bottom() - (offset.top + separatorV.height);
			firstPanel.top = offset.top + separatorV.height;
		}

		invalidateSize(firstPanel, separatorH, separatorV, secondPanel);

		// запоминаем дефолтный размер панели
		mFirstPanelSize = mFirstPanel->getSize();
	}

}
