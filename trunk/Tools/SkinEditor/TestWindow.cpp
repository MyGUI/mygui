/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "precompiled.h"
#include "TestWindow.h"

namespace tools
{

	TestWindow::TestWindow() :
		wraps::BaseLayout("TestWindow.layout")
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &TestWindow::notifyWindowButtonPressed);

		mMainWidget->setVisible(false);
	}

	TestWindow::~TestWindow()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed -= MyGUI::newDelegate(this, &TestWindow::notifyWindowButtonPressed);
	}

	void TestWindow::setVisible(bool _value)
	{
		if (mMainWidget->getVisible() != _value)
		{
			mMainWidget->setVisible(_value);

			if (_value)
			{
				MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
				addDialog(this);

				MyGUI::IntSize windowSize = mMainWidget->getSize();
				MyGUI::IntSize parentSize = mMainWidget->getParentSize();

				mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2);
			}
			else
			{
				MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
				removeDialog(this);
			}
		}
	}

	bool TestWindow::getVisible()
	{
		return mMainWidget->getVisible();
	}

	void TestWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		eventEndDialog(this, false);
	}

 } // namespace tools
