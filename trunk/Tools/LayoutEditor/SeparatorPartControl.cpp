#include "Precompiled.h"
#include "SeparatorPartControl.h"

namespace tools
{

	SeparatorPartControl::SeparatorPartControl(const std::string& _layout, MyGUI::Widget* _parent) :
		wraps::BaseLayout(_layout, _parent),
		mLeftPanel(nullptr),
		mRightPanel(nullptr),
		mSeparatorH(nullptr),
		mMinSizeLeft(0),
		mMinSizeRight(0),
		mChange(false)
	{
		assignWidget(mLeftPanel, "Left");
		assignWidget(mRightPanel, "Right");
		assignWidget(mSeparatorH, "SeparatorH");

		mMinSizeLeft = MyGUI::utility::parseValue<int>(mLeftPanel->getUserString("MinSize"));
		mMinSizeRight = MyGUI::utility::parseValue<int>(mRightPanel->getUserString("MinSize"));

		mSeparatorH->eventMouseButtonPressed += MyGUI::newDelegate(this, &SeparatorPartControl::notifyMouseButtonPressed);
		mSeparatorH->eventMouseButtonReleased += MyGUI::newDelegate(this, &SeparatorPartControl::notifyMouseButtonReleased);
		mSeparatorH->eventMouseDrag += MyGUI::newDelegate(this, &SeparatorPartControl::notifyMouseDrag);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &SeparatorPartControl::notifyFrameStart);
	}

	SeparatorPartControl::~SeparatorPartControl()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &SeparatorPartControl::notifyFrameStart);

		mSeparatorH->eventMouseButtonPressed -= MyGUI::newDelegate(this, &SeparatorPartControl::notifyMouseButtonPressed);
		mSeparatorH->eventMouseButtonReleased -= MyGUI::newDelegate(this, &SeparatorPartControl::notifyMouseButtonReleased);
		mSeparatorH->eventMouseDrag -= MyGUI::newDelegate(this, &SeparatorPartControl::notifyMouseDrag);
	}

	void SeparatorPartControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			mStartLeftPanel = mLeftPanel->getCoord();
			mStartRightPanel = mRightPanel->getCoord();
			mStartSeparatorH = mSeparatorH->getCoord();
			mStartMousePosition = MyGUI::InputManager::getInstance().getMousePosition();
		}
	}

	void SeparatorPartControl::notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
		}
	}

	void SeparatorPartControl::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			mChange = true;
		}
	}

	void SeparatorPartControl::notifyFrameStart(float _time)
	{
		if (mChange)
		{
			mChange = false;

			MyGUI::IntPoint mousePosition = MyGUI::InputManager::getInstance().getMousePosition();
			int delta = mousePosition.left - mStartMousePosition.left;

			MyGUI::IntCoord leftPanel = mStartLeftPanel;
			MyGUI::IntCoord rightPanel = mStartRightPanel;
			MyGUI::IntCoord separatorHPanel = mStartSeparatorH;

			leftPanel.width += delta;
			separatorHPanel.left += delta;
			rightPanel.left += delta;
			rightPanel.width -= delta;

			int diffLeft = mMinSizeLeft - leftPanel.width;
			if (diffLeft > 0)
			{
				leftPanel.width += diffLeft;
				separatorHPanel.left += diffLeft;
				rightPanel.left += diffLeft;
				rightPanel.width -= diffLeft;
			}

			int diffRight = mMinSizeRight - rightPanel.width;
			if (diffRight > 0)
			{
				leftPanel.width -= diffRight;
				separatorHPanel.left -= diffRight;
				rightPanel.left -= diffRight;
				rightPanel.width += diffRight;
			}

			mLeftPanel->setCoord(leftPanel);
			mRightPanel->setCoord(rightPanel);
			mSeparatorH->setCoord(separatorHPanel);
		}
	}

} // namespace tools
