/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "MainPane.h"

namespace tools
{

	MainPane::MainPane() :
		mTab(nullptr)
	{
	}

	void MainPane::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::Initialise(_parent, _place, _layoutName);

		assignWidget(mTab, "Tab");

		mTab->eventTabChangeSelect += MyGUI::newDelegate(this, &MainPane::notifyTabChangeSelect);

		notifyTabChangeSelect(mTab, 0);
	}

	MainPane::~MainPane()
	{
		mTab->eventTabChangeSelect -= MyGUI::newDelegate(this, &MainPane::notifyTabChangeSelect);
	}

	void MainPane::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	void MainPane::notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index)
	{
	}

} // namespace tools
