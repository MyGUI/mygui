/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "DialogManager.h"

template <> tools::DialogManager* MyGUI::Singleton<tools::DialogManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::DialogManager>::mClassTypeName("DialogManager");

namespace tools
{
	DialogManager::DialogManager()
	{
	}

	DialogManager::~DialogManager()
	{
	}

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
