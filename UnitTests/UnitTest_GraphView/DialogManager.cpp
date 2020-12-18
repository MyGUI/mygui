/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "DialogManager.h"

namespace tools
{

	MYGUI_SINGLETON_DEFINITION(DialogManager);

	void DialogManager::initialise()
	{
	}

	void DialogManager::shutdown()
	{
	}

	bool DialogManager::getAnyDialog()
	{
		return !mDialogs.empty();
	}

	void DialogManager::endTopDialog(bool _result)
	{
		if (!mDialogs.empty())
		{
			Dialog* item = mDialogs.back();
			item->eventEndDialog(item, _result);
		}
	}

	void DialogManager::_addDialog(Dialog* _modal)
	{
		mDialogs.push_back(_modal);
	}

	void DialogManager::_removeDialog(Dialog* _modal)
	{
		VectorDialog::iterator item = std::find(mDialogs.begin(), mDialogs.end(), _modal);
		mDialogs.erase(item);
	}

} // namespace tools
