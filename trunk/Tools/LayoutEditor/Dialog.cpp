/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "precompiled.h"
#include "Dialog.h"

namespace tools
{

	std::vector<Dialog*> Dialog::mDialogs;

	Dialog::Dialog()
	{
	}

	Dialog::~Dialog()
	{
	}

	void Dialog::addDialog(Dialog* _modal)
	{
		mDialogs.push_back(_modal);
	}

	void Dialog::removeDialog(Dialog* _modal)
	{
		std::vector<Dialog*>::iterator item = std::find(mDialogs.begin(), mDialogs.end(), _modal);
		mDialogs.erase(item);
	}

	bool Dialog::getAnyDialog()
	{
		return !mDialogs.empty();
	}

	void Dialog::endTopDialog(bool _result)
	{
		if (!mDialogs.empty())
		{
			Dialog* item = mDialogs.back();
			item->eventEndDialog(item, _result);
		}
	}

 } // namespace tools
